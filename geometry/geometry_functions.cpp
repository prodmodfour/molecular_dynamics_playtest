#include "geometry_functions.h"
namespace geometry {

Vector calculate_unit_vector_from_point1_to_point2(Vector point1, Vector point2)
{
    double x = point2.x - point1.x;
    double y = point2.y - point1.y;
    double z = point2.z - point1.z;
    double length = sqrt(x * x + y * y + z * z);
    return Vector{x / length, y / length, z / length};
}

std::vector<Vector> determine_cuboid_corners(std::vector<atoms::Atom> atoms)
{
double min_x = std::numeric_limits<double>::infinity();
double max_x = -std::numeric_limits<double>::infinity();
double min_y = std::numeric_limits<double>::infinity();
double max_y = -std::numeric_limits<double>::infinity();
double min_z = std::numeric_limits<double>::infinity();
double max_z = -std::numeric_limits<double>::infinity();

for (auto &atom : atoms)
{
    double x = atom.x;
    double y = atom.y;
    double z = atom.z;

    if (x < min_x) min_x = x;
    if (x > max_x) max_x = x;
    if (y < min_y) min_y = y;
    if (y > max_y) max_y = y;
    if (z < min_z) min_z = z;
    if (z > max_z) max_z = z;
}

std::vector<Vector> corners;
corners.reserve(8);

// The typical arrangement is:
corners.emplace_back(min_x, min_y, min_z); // #0
corners.emplace_back(min_x, min_y, max_z); // #1
corners.emplace_back(min_x, max_y, min_z); // #2
corners.emplace_back(min_x, max_y, max_z); // #3
corners.emplace_back(max_x, min_y, min_z); // #4
corners.emplace_back(max_x, min_y, max_z); // #5
corners.emplace_back(max_x, max_y, min_z); // #6
corners.emplace_back(max_x, max_y, max_z); // #7
    return corners;
}


}