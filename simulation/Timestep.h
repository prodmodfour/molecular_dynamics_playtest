#pragma once

#include "../atoms/Atom.h"
#include "Config.h"
#include <vector>

namespace simulation {

class Timestep
{
    public:
        simulation::Config config;
        std::vector<atoms::Atom> atoms;
        double kinetic_energy;
        double potential_energy;
        double time;

        Timestep(simulation::Config config, std::vector<atoms::Atom> atoms, double kinetic_energy, double potential_energy, double time)
        {
            this->config = config;
            this->atoms = atoms;
            this->kinetic_energy = kinetic_energy;
            this->potential_energy = potential_energy;
            this->time = time;
        }

        Timestep()
        {
            this->config = simulation::Config();
            this->atoms = std::vector<atoms::Atom>();
            this->kinetic_energy = 0;
            this->potential_energy = 0;
            this->time = 0;
        }


};

}