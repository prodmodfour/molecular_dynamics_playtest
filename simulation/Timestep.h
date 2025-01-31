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

        Timestep(Config config, std::vector<Atom> atoms, TotalEnergy total_energy)
        {
            this->config = config;
            this->atoms = atoms;
            this->total_energy = total_energy;
        }
}
