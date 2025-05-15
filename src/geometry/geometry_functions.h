#pragma once
#include "Vector.h"
#include "Plane.h"
#include "../atoms/Atom.h"
#include <vector>
#include <limits>
#include <cmath>


namespace geometry {

Vector calculate_unit_vector_from_point1_to_point2(Vector point1, Vector point2);

std::vector<Vector> determine_cuboid_corners(std::vector<atoms::Atom> atoms);


}