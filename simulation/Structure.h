#pragma once
#include <vector>
#include "Atom.h"
#include <string>
#include "Vector.h"
#include <map>
#include "Plane.h"

class Structure
{
    public:
        Structure(std::vector<Atom> atoms, std::string shape, std::vector<Vector> corners, std::map<std::string, Plane> planes, Vector center)
        {
            this->atoms = atoms;
            this->shape = shape;
            this->corners = corners;
            this->planes = planes;
            this->center = center;
        }
    
    std::vector<Atom> get_atoms()
    {
        return this->atoms;
    }

    std::string get_shape()
    {
        return this->shape;
    }

    std::vector<Vector> get_corners()
    {
        return this->corners;
    }

    std::map<std::string, Plane> get_planes()
    {
        return this->planes;
    }

    Vector get_center()
    {
        return this->center;
    }

    private:
        std::vector<Atom> atoms;
        std::string shape;
        std::vector<Vector> corners;
        std::map<std::string, Plane> planes;


        Vector center;


};





