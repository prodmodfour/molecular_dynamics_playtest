#pragma once
#include <vector>
#include "Atom.h"
#include <string>
#include "Cuboid.h"
#include "Vector.h"
#include <map>
#include "Plane.h"
#include <stdexcept>

class Structure
{
    public:
        Structure(std::vector<Atom> atoms, std::string shape)
        {
            this->atoms = atoms;
            this->shape = shape;

            if (shape == "cuboid")
            {

            }
            else
            {
                throw std::invalid_argument("Invalid shape");
            }
        }
        std::vector<Atom> atoms;
        std::string shape;
        std::vector<Vector> corners;
        std::map<std::string, Plane> planes;
        Vector center;


};





