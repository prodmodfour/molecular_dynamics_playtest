#pragma once
#include "Atom.h"
#include <vector>

class PotentialEnergyCalculator
{
    public:
        PotentialEnergyCalculator(std::vector<Atom> &atoms)
        {
            this->atoms = atoms;
        }

        std::vector<Atom> atoms;

        double calculate_potential_energy(Config config)
        {
            double potential_energy = 0;
            return potential_energy;
        }
};
