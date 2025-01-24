#pragma once
#include "Atom.h"
#include <vector>

class VelocityUpdater
{
    public:
        VelocityUpdater(std::vector<Atom> &atoms)
        {
            this->atoms = atoms;
        }
}

