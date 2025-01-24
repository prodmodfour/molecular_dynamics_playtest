#pragma once
#include "Vector.h"

class Plane
{
    public:
        Plane(Vector corner_1, Vector corner_2, Vector corner_3, Vector corner_4)
        {
            this->corner_1 = corner_1;
            this->corner_2 = corner_2;
            this->corner_3 = corner_3;
            this->corner_4 = corner_4;
            x_midpoint = corner_2.x - corner_1.x;
            y_midpoint = corner_3.y - corner_1.y;
            this->center = Vector{x_midpoint, y_midpoint, corner_1.z};
        };
        Vector normal;
        Vector corner_1;
        Vector corner_2;
        Vector corner_3;
        Vector corner_4;
        Vector center;
};

