#include "Plane.h"

namespace geometry {

Plane::Plane()
{
};

Plane::Plane(geometry::Vector corner_1, geometry::Vector corner_2, geometry::Vector corner_3, geometry::Vector corner_4)
{
    this->corners = std::vector<geometry::Vector>();
    this->corners.push_back(corner_1);
    this->corners.push_back(corner_2);
    this->corners.push_back(corner_3);
    this->corners.push_back(corner_4);

    double highest_x = -std::numeric_limits<double>::infinity();
    double highest_y = -std::numeric_limits<double>::infinity();
    double highest_z = -std::numeric_limits<double>::infinity();
    double lowest_x  =  std::numeric_limits<double>::infinity();
    double lowest_y  =  std::numeric_limits<double>::infinity();
    double lowest_z  =  std::numeric_limits<double>::infinity();

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
    this->center = geometry::Vector{x_midpoint, y_midpoint, z_midpoint};

};

geometry::Vector Plane::get_center()
{
    return this->center;
}

std::vector<geometry::Vector> Plane::get_corners()
{
    return this->corners;
}




}