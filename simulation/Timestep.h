#pragma once

#include "../atoms/Atom.h"
#include "Config.h"
#include "../physics/TotalEnergy.h"
#include <vector>

namespace simulation {

class Timestep
{
    public:
        simulation::Config config;
        std::vector<atoms::Atom> atoms;
        physics::TotalEnergy total_energy;
        double time;

        Timestep(simulation::Config config, std::vector<atoms::Atom> atoms, physics::TotalEnergy total_energy, double time)
        {
            this->config = config;
            this->atoms = atoms;
            this->total_energy = total_energy;
            this->time = time;
        }

        Timestep()
        {
            this->config = simulation::Config();
            this->atoms = std::vector<atoms::Atom>();
            this->total_energy = physics::TotalEnergy();
            this->time = 0;
        }


};

}