#ifndef __SimulationData_h
#define __SimulationData_h
#include <vector>
#include <Atom.h>
#include "Settings.h"
#include <mutex>

#include <iostream>
#include <vector>
#include "Material.h"
#include <simulation.h>

std::mutex timeline_mutex;

class Frame
{
    public:
        const std::vector<Atom> all_atoms;
        const double ke;
        const double pe;
        const double te;
        const double time;

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

        }

        void add_frame(Frame frame)
        {
            std::lock_guard<std::mutex> lock(timeline_mutex); 
            timeline.push_back(frame);
        }

        // Function to move forward
        bool move_forward() 
        {
            if (curent_index < timeline.size() - 1) 
            {
                curent_index++;
                return true;  // Moved forward successfully
            }
            return false; 
        }

        // Function to move backward
        bool move_backward() 
        {
            if (curent_index > 0) {
                curent_index--;
                return true;  // Moved backward successfully
            }
            return false;  
        }


        Frame get_current_frame() 
        {
            return timeline[curent_index];
        }

        Snapshot get_latest_frame()
        {
            return timeline.back();
        }

        bool ready()
        {
            return current_position <  total_frames;
        }

    
};


#endif