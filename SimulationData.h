#ifndef __SimulationData_h
#define __SimulationData_h
#include <vector>
#include <cmath>
#include "Atom.h"
#include "SimulationConfig.h"
#include <mutex>
#include "MDVisualiser.h"

extern std::mutex timeline_mutex;
class Frame
{
    public:
        std::vector<Atom> all_atoms;
        double ke;
        double pe;
        double te;
        double time;

        Frame(std::vector<Atom> all_atoms, double ke, double pe, double te, double time)
        {
            this->all_atoms = all_atoms;
            this->ke = ke;
            this->pe = pe;
            this->te = te;
            this->time = time;
        }

        Frame()
        {

        }
};


class SimulationData
{
    public:
        std::vector<Frame> timeline;
        int current_index;
        int max_buffer_frames;
        SimulationConfig config;


        SimulationData(std::vector<Atom> all_atoms, SimulationConfig& config)
        {
            current_index = 0;
            zero_forces(all_atoms);
            double pe = evaluate_forces(all_atoms, config);
            double ke = 0;
            for (int i = 0; i < all_atoms.size(); i++)
            {
                ke += all_atoms[i].ke;
            }
            double time = 0.0;
            
            Frame frame(all_atoms, ke, pe, ke + pe, time);
            add_frame(frame);
            max_buffer_frames = 10000;
            this->config = config;
        }

        void add_frame(Frame frame)
        {
            std::lock_guard<std::mutex> lock(timeline_mutex); 
            timeline.push_back(frame);
        }

        // Function to move forward
        bool move_forward() 
        {
            std::lock_guard<std::mutex> lock(timeline_mutex); 
            if (current_index < timeline.size() - 1) 
            {
                current_index++;

                return true;  
            }

            return false; 
        }

        // Function to move backward
        bool move_backward() 
        {
            std::lock_guard<std::mutex> lock(timeline_mutex); 
            if (current_index > 0) {
                current_index--;
                return true;  // Moved backward successfully
            }
            return false;  
        }

        bool buffer_full()
        {
            std::lock_guard<std::mutex> lock(timeline_mutex); 
            return timeline.size() > current_index + max_buffer_frames;
        }


        Frame get_current_frame() 
        {
            std::lock_guard<std::mutex> lock(timeline_mutex); 
            return timeline[current_index];
        }

        Frame get_latest_frame()
        {
            std::lock_guard<std::mutex> lock(timeline_mutex); 
            if (!timeline.empty())
            {
                return timeline.back();
            }
            else
            {

                return Frame();
            }
        }

        void zero_forces(std::vector<Atom> &all_atoms)
        {
            for (int i = 0; i < all_atoms.size(); i++)
            {
                all_atoms[i].fx = 0;
                all_atoms[i].fy = 0;
                all_atoms[i].fz = 0;
            }
        }

        double evaluate_forces(std::vector<Atom>& all_atoms, SimulationConfig& config)
        {
            double epsilon = config.getEpsilon();
            double sigma = config.getSigma();
            double r_cutoff = config.getRcutoff();
            double r_cutoff_squared = r_cutoff * r_cutoff;
            double potential_energy = 0;
            // Find forces from every unique pair interaction
            for (int i = 0; i < all_atoms.size(); i++)
            {

                double xi = all_atoms[i].x;
                double yi = all_atoms[i].y;
                double zi = all_atoms[i].z;

                for (int j = i + 1; j < all_atoms.size(); j++)
                {
                    double xj = all_atoms[j].x;
                    double yj = all_atoms[j].y;
                    double zj = all_atoms[j].z;

                    double dx = xi - xj;
                    if (dx > r_cutoff)
                    {
                        continue;
                    }
                    double dy = yi - yj;
                    if (dy > r_cutoff)
                    {
                        continue;
                    }
                    double dz = zi - zj;
                    if (dz > r_cutoff)
                    {
                        continue;
                    }

                    double squared_distance = dx*dx + dy*dy + dz*dz;
                    if (squared_distance > r_cutoff_squared)
                    {
                        continue;
                    }

                    double distance = std::sqrt(squared_distance);

                    // Calculate force and potential between i and j
                    double sr2 = sigma * sigma / squared_distance;
                    double sr6 = sr2 * sr2 * sr2;
                    double sr12 = sr6 * sr6;

                    double reciprocal_distance = 1 / distance;
                    double unit_vector_x = dx * reciprocal_distance;
                    double unit_vector_y = dy * reciprocal_distance;
                    double unit_vector_z = dz * reciprocal_distance;

                    double potential_ij = sr12 - sr6;
                    double potential_energy = potential_ij;

                    double fij = (2 * sr12 - sr6) * reciprocal_distance;
                    double fxij = fij * unit_vector_x;
                    double fyij = fij * unit_vector_y;
                    double fzij = fij * unit_vector_z; 

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
                    all_atoms[i].fx *= 24 * epsilon;
                    all_atoms[i].fy *= 24 * epsilon;
                    all_atoms[i].fz *= 24 * epsilon;
                }
                // Multiply by energy factor
                potential_energy *= 4 * epsilon;

                // Convert to eV
                double j_per_mole_to_ev = config.j_per_mole_to_ev;
                potential_energy *= j_per_mole_to_ev;

                return potential_energy;

        }

    
};


#endif