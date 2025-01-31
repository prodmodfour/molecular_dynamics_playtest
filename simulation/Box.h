#pragma once
#include "Vector.h"
#include "Plane.h"

typedef struct
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


class Box
{
    public:
        Box(Vector corner_1, Vector corner_2, Vector corner_3, Vector corner_4, Vector corner_5, Vector corner_6, Vector corner_7, Vector corner_8)
        {
            this->corner_1 = corner_1;
            this->corner_2 = corner_2;
            this->corner_3 = corner_3;
            this->corner_4 = corner_4;
            this->corner_5 = corner_5;
            this->corner_6 = corner_6;
            this->corner_7 = corner_7;
            this->corner_8 = corner_8;

            this->top_plane = Plane(corner_1, corner_2, corner_3, corner_4);
            this->bottom_plane = Plane(corner_5, corner_6, corner_7, corner_8);
            this->left_plane = Plane(corner_1, corner_2, corner_6, corner_7);
            this->right_plane = Plane(corner_3, corner_4, corner_8, corner_5);
            this->front_plane = Plane(corner_1, corner_4, corner_8, corner_5);
            this->back_plane = Plane(corner_2, corner_3, corner_7, corner_6);
        }

        Vector corner_1;
        Vector corner_2;
        Vector corner_3;
        Vector corner_4;
        Vector corner_5;
        Vector corner_6;
        Vector corner_7;
        Vector corner_8;

        Plane top_plane;
        Plane bottom_plane;
        Plane left_plane;
        Plane right_plane;
        Plane front_plane;
        Plane back_plane;

        Vector center;

};


