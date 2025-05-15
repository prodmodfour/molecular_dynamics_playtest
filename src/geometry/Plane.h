#pragma once
#include "Vector.h"
#include <vector>
#include <limits>
#include <cmath>

namespace geometry {
class Plane
{
    public:
        Plane();

        Plane(geometry::Vector corner_1, geometry::Vector corner_2, geometry::Vector corner_3, geometry::Vector corner_4);
        
        geometry::Vector get_center();
        std::vector<geometry::Vector> get_corners();

        private:
        geometry::Vector normal;
        std::vector<geometry::Vector> corners;
        geometry::Vector center;
};

}