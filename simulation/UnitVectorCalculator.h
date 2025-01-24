#pragma once
#include <tuple>
#include "Vector.h"

class UnitVectorCalculator
{
    public:
        UnitVectorCalculator()
        {

        }
    
    Vector calculate_unit_vector_between_two_points(Vector point1, Vector point2)
    {
        double x = point2.x - point1.x;
        double y = point2.y - point1.y;
        double z = point2.z - point1.z;
        double length = sqrt(x * x + y * y + z * z);
        return Vector{x / length, y / length, z / length};
    }
};
