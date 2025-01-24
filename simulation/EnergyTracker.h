#pragma once
#include <vector>

class TotalEnergy
{
    public:
        double kinetic;
        double potential;
};

class EnergyTracker
{
    public:
        EnergyTracker()
        {
            this->total_energy_history = std::vector<TotalEnergy>();
        }

        std::vector<TotalEnergy> total_energy_history;

        void add_total_energy_to_history(TotalEnergy total_energy)
        {
            total_energy_history.push_back(total_energy);
        }
};

