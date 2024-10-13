#ifndef __SimulationData_h
#define __SimulationData_h
#include <vector>
#include "Atom.h"
#include "Settings.h"
#include <mutex>

#include <iostream>
#include <vector>
#include "simulation.h"

std::mutex timeline_mutex;

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


        SimulationData(std::vector<Atom> all_atoms, Settings settings)
        {
            current_index = 0;
            zero_forces(all_atoms);
            double pe = evaluate_forces(all_atoms, settings);
            double ke = 0;
            for (int i = 0; i < all_atoms.size(); i++)
            {
                ke += all_atoms[i].ke;
            }
            double time = 0.0;
            
            Frame frame(all_atoms, ke, pe, ke + pe, time);
            add_frame(frame);
            max_buffer_frames = 1000;

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