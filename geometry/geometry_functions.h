#pragma once
#include "Vector.h"
#include "Plane.h"
#include "Atom.h"
#include <vector>

namespace geometry {

Vector calculate_unit_vector_between_two_points(Vector point1, Vector point2)
{
    double x = point2.x - point1.x;
    double y = point2.y - point1.y;
    double z = point2.z - point1.z;
    double length = sqrt(x * x + y * y + z * z);
    return Vector{x / length, y / length, z / length};
}

std::vector<Vector> determine_cuboid_corners(std::vector<Atom> atoms)
{
    std::vector<Vector> corners(8, Vector{0, 0, 0});

    for (auto atom : atoms)
    {
        if (atom.x < corners[0].x)
            corners[0] = atom.position;
        if (atom.x > corners[1].x)
            corners[1] = atom.position;
        if (atom.y < corners[2].y)
            corners[2] = atom.position;
        if (atom.y > corners[3].y)
            corners[3] = atom.position;
        if (atom.z < corners[4].z)
            corners[4] = atom.position;
        if (atom.z > corners[5].z)
            corners[5] = atom.position;
        if (atom.z < corners[6].z)
            corners[6] = atom.position;
        if (atom.z > corners[7].z)
            corners[7] = atom.position;
    }

    return corners;
}



}