#pragma once
#include <tuple>
#include "UnitVector.h"

class UnitVectorCalculator
{
    public:
        UnitVectorCalculator()
        {

        }
    
    UnitVector calculate_unit_vector_between_two_points(std::tuple<double, double, double> point1, std::tuple<double, double, double> point2)
    {
        double x = point2[0] - point1[0];
        double y = point2[1] - point1[1];
        double z = point2[2] - point1[2];
        double length = sqrt(x * x + y * y + z * z);
        return UnitVector{x / length, y / length, z / length};
    }
};
