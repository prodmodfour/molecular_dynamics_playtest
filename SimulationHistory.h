#ifndef __SimulationHistory_h
#define __SimulationHistory_h

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <mutex>
#include <memory>
#include "Atom.h"
#include "Settings.h"
#include "simulation.h"

std::mutex timeline_mutex;
std::mutex branch_mutex;

class Snapshot {
public:
    std::vector<Atom> all_atoms;
    double ke;
    double pe;
    double te;
    double time;
    std::string branch_id;  // Added for branch tracking

    Snapshot(std::vector<Atom> all_atoms, double ke, double pe, double te, double time, std::string branch_id = "main")
        : all_atoms(all_atoms), ke(ke), pe(pe), te(te), time(time), branch_id(branch_id) {}

    Snapshot() : ke(0), pe(0), te(0), time(0), branch_id("main") {}

    // Comparison operator for time-based sorting
    bool operator<(const Snapshot& other) const {
        return time < other.time;
    }
};

class SimulationBranch {
public:
    using iterator = std::vector<Snapshot>::iterator;
    using const_iterator = std::vector<Snapshot>::const_iterator;

    std::string parent_branch_id;
    double branch_start_time;  // When this branch diverges from parent

    SimulationBranch(const std::string& id = "main", const std::string& parent = "", double start_time = 0.0)
        : branch_id(id), parent_branch_id(parent), branch_start_time(start_time) {}

    // Iterator access
    iterator begin() { return timeline.begin(); }
    iterator end() { return timeline.end(); }
    const_iterator begin() const { return timeline.begin(); }
    const_iterator end() const { return timeline.end(); }
    const_iterator cbegin() const { return timeline.cbegin(); }
    const_iterator cend() const { return timeline.cend(); }

    // Branch information
    std::string get_id() const { return branch_id; }
    double get_branch_point_time() const { return branch_point_time; }
    
    // Timeline management
    void add_snapshot(const Snapshot& snapshot) {
        timeline.push_back(snapshot);
    }

    Snapshot& get_snapshot_at(size_t index) {
        return timeline.at(index);
    }

    const Snapshot& get_snapshot_at(size_t index) const {
        return timeline.at(index);
    }

    size_t size() const { return timeline.size(); }
    bool empty() const { return timeline.empty(); }

    Snapshot& back() { return timeline.back(); }
    const Snapshot& back() const { return timeline.back(); }

private:
    std::vector<Snapshot> timeline;
    std::string branch_id;
    double branch_point_time;
    friend class SimulationHistory;
};

class SimulationHistory {
private:
    std::map<std::string, std::unique_ptr<SimulationBranch>> branches;
    std::string active_branch_id;
    size_t current_index;
    int max_buffer_frames;
    Settings current_settings;
    std::map<std::string, Settings> branch_settings;

public:
    SimulationHistory(std::vector<Atom>& all_atoms, Settings settings)
        : active_branch_id("main"), current_index(0), max_buffer_frames(10000), current_settings(settings) {
        // Initialize main branch
        branches["main"] = std::make_unique<SimulationBranch>("main");
        branch_settings["main"] = settings;
        
        // Create initial snapshot
        zero_forces(all_atoms);
        double pe = evaluate_forces(all_atoms, settings);
        double ke = 0;
        for (const auto& atom : all_atoms) {
            ke += atom.ke;
        }
        
        Snapshot initial_frame(all_atoms, ke, pe, ke + pe, 0.0, "main");
        add_frame(initial_frame);
    }

    // Branch management
   std::string create_branch(const std::string& new_branch_id, const Settings& branch_settings) {
        // First get current state while holding timeline_mutex
        Snapshot current_frame;
        {
            std::lock_guard<std::mutex> lock(timeline_mutex);
            current_frame = get_current_frame();
        }

        // Then create branch while holding branch_mutex
        {
            std::lock_guard<std::mutex> branch_lock(branch_mutex);
            
            // Validate branch ID
            if (new_branch_id.empty() || branches.find(new_branch_id) != branches.end()) {
                std::cerr << "Invalid or duplicate branch ID" << std::endl;
                return "";
            }

            try {
                // Deep copy the atoms for the new branch
                std::vector<Atom> copied_atoms = current_frame.all_atoms;
                
                // Create new branch
                auto new_branch = std::make_unique<SimulationBranch>(
                    new_branch_id,
                    active_branch_id,
                    current_frame.time
                );
                
                // Initialize the branch with the current frame
                Snapshot initial_branch_frame(
                    copied_atoms,
                    current_frame.ke,
                    current_frame.pe,
                    current_frame.te,
                    current_frame.time,
                    new_branch_id
                );
                new_branch->add_snapshot(initial_branch_frame);

                // Store branch settings and branch
                this->branch_settings[new_branch_id] = branch_settings;
                branches[new_branch_id] = std::move(new_branch);

                // Switch to the new branch
                active_branch_id = new_branch_id;
                current_index = 0;

                return new_branch_id;
            }
            catch (const std::exception& e) {
                std::cerr << "Error creating branch: " << e.what() << std::endl;
                return "";
            }
        }
    }

    bool switch_branch(const std::string& branch_id) {
        std::lock_guard<std::mutex> branch_lock(branch_mutex);
        
        auto branch_it = branches.find(branch_id);
        if (branch_it == branches.end()) {
            std::cerr << "Branch not found: " << branch_id << std::endl;
            return false;
        }

        try {
            double current_time;
            {
                std::lock_guard<std::mutex> timeline_lock(timeline_mutex);
                current_time = get_current_frame().time;
            }
            
            auto& new_branch = *branch_it->second;
            
            auto it = std::lower_bound(
                new_branch.begin(), new_branch.end(),
                Snapshot({}, 0, 0, 0, current_time),
                [](const Snapshot& a, const Snapshot& b) { return a.time < b.time; }
            );

            if (it != new_branch.end()) {
                active_branch_id = branch_id;
                current_index = std::distance(new_branch.begin(), it);
                return true;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error switching branch: " << e.what() << std::endl;
        }
        return false;
    }

    Settings& get_branch_settings(const std::string& branch_id) {
        std::lock_guard<std::mutex> lock(timeline_mutex);
        return branch_settings[branch_id];
    }

    const Settings& get_current_settings() const {
        return branch_settings.at(active_branch_id);
    }

    // Frame management
    void add_frame(const Snapshot& frame) {
        std::lock_guard<std::mutex> lock(timeline_mutex);
        auto& current_branch = *branches[active_branch_id];
        current_branch.add_snapshot(frame);
    }

   bool move_forward() {
        std::lock_guard<std::mutex> lock(timeline_mutex);
        auto& current_branch = *branches[active_branch_id];
        
        // Handle transition back to child branch if exists
        for (const auto& [branch_id, branch] : branches) {
            if (branch->parent_branch_id == active_branch_id && 
                current_branch.timeline[current_index].time >= branch->branch_start_time) {
                active_branch_id = branch_id;
                break;
            }
        }
        
        if (current_index < current_branch.timeline.size() - 1) {
            current_index++;
            return true;
        }
        return false;
    }

  bool move_backward() {
        std::lock_guard<std::mutex> lock(timeline_mutex);
        if (current_index > 0) {
            current_index--;
            
            // Check if we need to switch to parent branch
            auto& current_branch = *branches[active_branch_id];
            if (current_branch.parent_branch_id != "" && 
                get_current_frame().time < current_branch.branch_start_time) {
                    
                active_branch_id = current_branch.parent_branch_id;
            }
            return true;
        }
        return false;
    }

    bool buffer_full() {
        std::lock_guard<std::mutex> lock(timeline_mutex);
        auto& current_branch = *branches[active_branch_id];
        return current_branch.size() > current_index + max_buffer_frames;
    }

    // Frame access
    Snapshot get_current_frame() {
        std::lock_guard<std::mutex> lock(timeline_mutex);
        auto& current_branch = *branches[active_branch_id];
        
        if (current_branch.parent_branch_id != "" && 
            current_branch.timeline.size() > 0 &&
            current_branch.timeline[current_index].time < current_branch.branch_start_time) {
            return branches[current_branch.parent_branch_id]->get_snapshot_at(current_index);
        }

        return current_branch.get_snapshot_at(current_index);
    }

    Snapshot get_latest_frame() {
        std::lock_guard<std::mutex> lock(timeline_mutex);
        auto& current_branch = *branches[active_branch_id];
        if (!current_branch.empty()) {
            return current_branch.back();
        }
        return Snapshot();
    }

    // Branch information
    std::vector<std::string> get_available_branches() const {
        std::vector<std::string> branch_ids;
        for (const auto& [id, _] : branches) {
            branch_ids.push_back(id);
        }
        return branch_ids;
    }

    std::string get_active_branch_id() const {
        return active_branch_id;
    }

    // Query methods for visualization
    std::vector<std::pair<double, std::vector<std::string>>> get_branch_points() const {
        std::map<double, std::vector<std::string>> points;
        for (const auto& [id, branch] : branches) {
            points[branch->get_branch_point_time()].push_back(id);
        }
        return std::vector<std::pair<double, std::vector<std::string>>>(
            points.begin(), points.end());
    }

private:
    void zero_forces(std::vector<Atom>& all_atoms) {
        for (auto& atom : all_atoms) {
            atom.fx = 0;
            atom.fy = 0;
            atom.fz = 0;
        }
    }



        double evaluate_forces(std::vector<Atom> &all_atoms, Settings settings)
        {
            double xi, yi, zi;
            double xj, yj, zj;
            double dx, dy, dz;
            double fxij, fyij, fzij, fij;
            double squared_distance, distance, sigma_squared;
            double sr2, sr6, sr12, epsilon4, epsilon24;
            double potential_ij, potential_energy, unit_vector_x, unit_vector_y, unit_vector_z;
            double reciprocal_distance;

            potential_energy = 0;


            double epsilon = settings.get_epsilon();
            double sigma = settings.get_sigma();
            // We will only ever have to deal with Cu-Cu interactions
            epsilon4 = 4 * epsilon;
            epsilon24 = 24 * epsilon;
            sigma_squared = sigma * sigma;

            double r_cutoff = settings.get_rcutoff();
            double r_cutoff_squared = r_cutoff * r_cutoff;
            // Find forces from every unique pair interaction
            for (int i = 0; i < all_atoms.size(); i++)
            {

                xi = all_atoms[i].x;
                yi = all_atoms[i].y;
                zi = all_atoms[i].z;

                for (int j = i + 1; j < all_atoms.size(); j++)
                {
                    xj = all_atoms[j].x;
                    yj = all_atoms[j].y;
                    zj = all_atoms[j].z;

                    dx = xi - xj;
                    if (dx > r_cutoff)
                    {
                        continue;
                    }
                    dy = yi - yj;
                    if (dy > r_cutoff)
                    {
                        continue;
                    }
                    dz = zi - zj;
                    if (dz > r_cutoff)
                    {
                        continue;
                    }

                    squared_distance = dx*dx + dy*dy + dz*dz;
                    if (squared_distance > r_cutoff_squared)
                    {
                        continue;
                    }

                    distance = std::sqrt(squared_distance);

                    // Calculate force and potential between i and j
                    sr2 = sigma_squared / squared_distance;
                    sr6 = sr2 * sr2 * sr2;
                    sr12 = sr6 * sr6;

                    reciprocal_distance = 1 / distance;
                    unit_vector_x = dx * reciprocal_distance;
                    unit_vector_y = dy * reciprocal_distance;
                    unit_vector_z = dz * reciprocal_distance;

                    potential_ij = sr12 - sr6;
                    potential_energy += potential_ij;

                    fij = (2 * sr12 - sr6) * reciprocal_distance;
                    fxij = fij * unit_vector_x;
                    fyij = fij * unit_vector_y;
                    fzij = fij * unit_vector_z; 

                    /* Sum the forces on i due to all j */
                    all_atoms[i].fx += fxij;
                    all_atoms[i].fy += fyij;
                    all_atoms[i].fz += fzij;

                    /* Sum the forces on j due to i in ij direction */
                    all_atoms[j].fx -= fxij;
                    all_atoms[j].fy -= fyij;
                    all_atoms[j].fz -= fzij;          
                }


                
                

            }
                for (int i = 0; i < all_atoms.size(); i++)
                {
                    all_atoms[i].fx *= epsilon24;
                    all_atoms[i].fy *= epsilon24;
                    all_atoms[i].fz *= epsilon24;
                }
                // Multiply by energy factor
                potential_energy *= epsilon4;

                // Convert to eV
                double j_per_mole_to_ev = settings.get_j_per_mole_to_ev();
                potential_energy *= j_per_mole_to_ev;

                return potential_energy;

        }

    
};


#endif