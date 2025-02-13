#pragma once
#include "Vector.h"
#include <vector>

namespace geometry {
class Plane
{
    public:
        Plane(Vector corner_1, Vector corner_2, Vector corner_3, Vector corner_4)
        {
            this->corners.push_back(corner_1);
            this->corners.push_back(corner_2);
            this->corners.push_back(corner_3);
            this->corners.push_back(corner_4);
            double highest_x, highest_y, highest_z, lowest_x, lowest_y, lowest_z;
            highest_x = 0;
            highest_y = 0;
            highest_z = 0;
            lowest_x = 0;
            lowest_y = 0;
            lowest_z = 0;

            for (auto corner : this->corners)
            {
                if (corner.x > highest_x) highest_x = corner.x;
                if (corner.y > highest_y) highest_y = corner.y;
                if (corner.z > highest_z) highest_z = corner.z;
                if (corner.x < lowest_x) lowest_x = corner.x;
                if (corner.y < lowest_y) lowest_y = corner.y;
                if (corner.z < lowest_z) lowest_z = corner.z;
            }
            
            double x_midpoint = (highest_x + lowest_x) / 2;
            double y_midpoint = (highest_y + lowest_y) / 2;
            double z_midpoint = (highest_z + lowest_z) / 2;
            this->center = Vector{x_midpoint, y_midpoint, z_midpoint};
        };
        Vector normal;
        std::vector<Vector> corners;
        Vector center;
};

}