#pragma once
#include <vector>

class TotalEnergy
{
    public:
        TotalEnergy(double kinetic, double potential)
        {
            this->kinetic = kinetic;
            this->potential = potential;
        }

        double kinetic;
        double potential;
};

