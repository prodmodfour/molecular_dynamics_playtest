#pragma once
#include <string>
#include "../geometry/Vector.h"
#include <vector>

namespace atoms {

class Atom
{

    public:

        double x, y, z;
        double fx, fy, fz;
        double vx, vy, vz;
        double mass;
        double radius;
        double kinetic_energy;
        std::string type;
        std::string name; 
        std::string parent_structure;

        Atom(std::string type, double mass, double radius)
        {
            x = 0;
            y = 0;
            z = 0;
            fx = 0;
            fy = 0;
            fz = 0;
            vx = 0;
            vy = 0;
            vz = 0;
            this->mass = mass;
            this->radius = radius;
            kinetic_energy = 0;
            this->type = type;
            name = "atom";
        }

        geometry::Vector get_position()
        {
            return geometry::Vector(x, y, z);
        }
};

}