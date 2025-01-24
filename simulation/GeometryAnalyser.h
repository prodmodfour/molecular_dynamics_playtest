#pragma once
#include <vector>
#include <tuple>
#include "Vector.h"
#include "Box.h"

class GeometryAnalyser
{
    public:
        GeometryAnalyser();

        Box determine_box(std::vector<Atom> atoms)
        {
            // Set initial values for the box corner atoms
            Box box;
            box.corner_1 = Vector{0, 0, 0};
            box.corner_2 = Vector{0, 0, 0};
            box.corner_3 = Vector{0, 0, 0};
            box.corner_4 = Vector{0, 0, 0};
            box.corner_5 = Vector{0, 0, 0};
            box.corner_6 = Vector{0, 0, 0};
            box.corner_7 = Vector{0, 0, 0};
            box.corner_8 = Vector{0, 0, 0};

            for (auto atom : atoms)
            {
                if (atom.x < box.corner_1.x)
                    box.corner_1 = Vector{atom.x, atom.y, atom.z};
                if (atom.x > box.corner_2.x)
                    box.corner_2 = Vector{atom.x, atom.y, atom.z};
                if (atom.y < box.corner_3.y)
                    box.corner_3 = Vector{atom.x, atom.y, atom.z};
                if (atom.y > box.corner_4.y)
                    box.corner_4 = Vector{atom.x, atom.y, atom.z};
                if (atom.z < box.corner_5.z)
                    box.corner_5 = Vector{atom.x, atom.y, atom.z};
                if (atom.z > box.corner_6.z)
                    box.corner_6 = Vector{atom.x, atom.y, atom.z};
                if (atom.z > box.corner_7.z)
                    box.corner_7 = Vector{atom.x, atom.y, atom.z};
                if (atom.z < box.corner_8.z)
                    box.corner_8 = Vector{atom.x, atom.y, atom.z};
            }

            return box;
        }

        Plane find_plane_from_box(Box box, std::string plane_name)
        {

        }

        Vector calculate_center
};