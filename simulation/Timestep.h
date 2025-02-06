#pragma once

#include "Atom.h"
#include "Config.h"
#include "TotalEnergy.h"
#include <vector>

class Timestep
{
    public:
        Config config;
        std::vector<Atom> atoms;
        TotalEnergy total_energy;
        double time;

        Timestep(Config config, std::vector<Atom> atoms, TotalEnergy total_energy, double time)
        {
            this->config = config;
            this->atoms = atoms;
            this->total_energy = total_energy;
            this->time = time;
        }


};
