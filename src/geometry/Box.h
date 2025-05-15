#pragma once

#include "Vector.h"
#include "Plane.h"
#include "../atoms/Atom.h"  
#include <vector>
#include "geometry_functions.h"

namespace geometry {

class Box
{
    public:
        Box();

        Box(std::vector<atoms::Atom> &atoms);

        std::vector<geometry::Vector> get_corners();
        geometry::Plane get_top_plane();
        geometry::Plane get_bottom_plane();
        geometry::Plane get_left_plane();
        geometry::Plane get_right_plane();
        geometry::Plane get_front_plane();
        geometry::Plane get_back_plane();
        geometry::Vector get_center();

    private:

        std::vector<geometry::Vector> corners;
        geometry::Plane top_plane;
        geometry::Plane bottom_plane;
        geometry::Plane left_plane;
        geometry::Plane right_plane;
        geometry::Plane front_plane;
        geometry::Plane back_plane;
        geometry::Vector center;

};

}