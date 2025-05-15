#pragma once
#include <string>
#include "../geometry/Vector.h"
#include <vector>

namespace atoms {

typedef struct Atom
{
        double x, y, z;
        double fx, fy, fz;
        double vx, vy, vz;
        double mass;
        double radius;
        double kinetic_energy;
        std::string type;
        std::string name; 
        std::string parent_structure;

        Atom(std::string type, double mass, double radius);

        geometry::Vector get_position();

} Atom;

}