#pragma once
#include "Vector.h"
#include "Plane.h"

typedef struct Box  
{
    Vector corner_1;
    Vector corner_2;
    Vector corner_3;
    Vector corner_4;
    Vector corner_5;
    Vector corner_6;
    Vector corner_7;
    Vector corner_8;
    Vector center;
    Plane top_plane;
    Plane bottom_plane;
    Plane left_plane;
    Plane right_plane;
    Plane front_plane;
    Plane back_plane;

} Box;
