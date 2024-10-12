#ifndef __SimulationData_h
#define __SimulationData_h
#include <vector>
#include <Atom.h>
#include "Settings.h"
#include <mutex>

#include <iostream>
#include <vector>
#include "Material.h"

std::mutex timeline_mutex;

class SimulationData
{
    public:
        std::vector<Frame> timeline;
        int current_position;
        int total_frames;

        SimulationData()
        {
            current_position = 0;
            total_frames = 0;
        }

        void add_frame(Frame frame)
        {
            std::lock_guard<std::mutex> lock(timeline_mutex); 
            timeline.push_back(frame);
            total_frames++;
        }

        Frame get_next_frame()
        {
            std::lock_guard<std::mutex> lock(timeline_mutex);
            current_position++;
            return timeline[current_position - 1];
        }

        bool ready()
        {
            return current_position <  total_frames;
        }
    
};

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
};

#endif