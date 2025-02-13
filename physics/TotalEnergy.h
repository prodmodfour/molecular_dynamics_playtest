#pragma once
#include <vector>

namespace physics {

class TotalEnergy
{
    public:
        TotalEnergy(double kinetic, double potential)
        {
            this->kinetic = kinetic;
            this->potential = potential;
        }

        TotalEnergy()
        {
            this->kinetic = 0;
            this->potential = 0;
        }

        double kinetic;
        double potential;
};

}