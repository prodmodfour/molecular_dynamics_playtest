#pragma once
#include <vector>
#include "../atoms/Atom.h"
#include <string>
#include "Vector.h"
#include <map>
#include "Plane.h"

namespace geometry {

class StructureGeometry
{
    public:
        StructureGeometry(std::vector<atoms::Atom> atoms, std::string shape_name, std::vector<Vector> corners, std::map<std::string, Plane> planes, Vector center)
        {
            this->atoms = atoms;
            this->shape_name = shape_name;
            this->corners = corners;
            this->planes = planes;
            this->center = center;
        }
    
    std::vector<atoms::Atom> get_atoms()
    {
        return this->atoms;
    }

    std::string get_shape_name()
    {
        return this->shape_name;
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
        std::vector<atoms::Atom> atoms;
        std::string shape_name;
        std::vector<Vector> corners;
        std::map<std::string, Plane> planes;


        Vector center;


};

}



