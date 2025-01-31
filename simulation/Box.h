#pragma once
#include "Vector.h"
#include "Plane.h"
#include "Atom.h"


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
        Box(std::vector<Atom> atoms)
        {

            this->corner_1 = Vector{0, 0, 0};
            this->corner_2 = Vector{0, 0, 0};
            this->corner_3 = Vector{0, 0, 0};
            this->corner_4 = Vector{0, 0, 0};
            this->corner_5 = Vector{0, 0, 0};
            this->corner_6 = Vector{0, 0, 0};
            this->corner_7 = Vector{0, 0, 0};
            this->corner_8 = Vector{0, 0, 0};

            for (auto atom : atoms)
            {
                if (atom.x < this->corner_1.x)
                    this->corner_1 = Vector{atom.x, atom.y, atom.z};
                if (atom.x > this->corner_2.x)
                    this->corner_2 = Vector{atom.x, atom.y, atom.z};
                if (atom.y < this->corner_3.y)
                    this->corner_3 = Vector{atom.x, atom.y, atom.z};
                if (atom.y > this->corner_4.y)
                    this->corner_4 = Vector{atom.x, atom.y, atom.z};
                if (atom.z < this->corner_5.z)
                    this->corner_5 = Vector{atom.x, atom.y, atom.z};
                if (atom.z > this->corner_6.z)
                    this->corner_6 = Vector{atom.x, atom.y, atom.z};
                if (atom.z > this->corner_7.z)
                    this->corner_7 = Vector{atom.x, atom.y, atom.z};
                if (atom.z < this->corner_8.z)
                    this->corner_8 = Vector{atom.x, atom.y, atom.z};
            }


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


