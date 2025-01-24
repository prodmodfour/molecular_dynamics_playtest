#pragma once
#include "Vector.h"

typedef struct Plane
{
    Vector normal;
    Vector corner_1;
    Vector corner_2;
    Vector corner_3;
    Vector corner_4;
    Vector center;
} Plane;
