#pragma once

#include <vector>
#include "Timestep.h"

namespace simulation
{
    class SimulationTimeline
    {
        public:
        std::vector<simulation::Timestep> past_timesteps;
        simulation::Timestep current_timestep;
        std::vector<simulation::Timestep> future_timesteps;

        SimulationTimeline(std::vector<simulation::Timestep> simulation_data)
        {
            past_timesteps = std::vector<simulation::Timestep>();
            future_timesteps = simulation_data;
            current_timestep = future_timesteps.front();
            future_timesteps.erase(future_timesteps.begin());
        }

        void add_timestep(const simulation::Timestep& timestep)
        {
            future_timesteps.push_back(timestep);
        }

        void add_timesteps(const std::vector<simulation::Timestep>& timesteps)
        {
            for (const auto& timestep : timesteps)
            {
                future_timesteps.push_back(timestep);
            }
        }

        void dump_future_timesteps()
        {
            future_timesteps.erase(future_timesteps.begin(), future_timesteps.end());
        }

        void progress()
        {
            past_timesteps.push_back(current_timestep);
            current_timestep = future_timesteps.front();
            future_timesteps.erase(future_timesteps.begin());
        }
        
    };
}