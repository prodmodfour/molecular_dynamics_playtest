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

            box.top_plane = Plane{box.corner_1, box.corner_2, box.corner_3, box.corner_4};
            box.bottom_plane = Plane{box.corner_5, box.corner_6, box.corner_7, box.corner_8};
            box.left_plane = Plane{box.corner_1, box.corner_2, box.corner_6, box.corner_7};
            box.right_plane = Plane{box.corner_3, box.corner_4, box.corner_8, box.corner_5};
            box.front_plane = Plane{box.corner_1, box.corner_4, box.corner_8, box.corner_5};
            box.back_plane = Plane{box.corner_2, box.corner_3, box.corner_7, box.corner_6};

            double highest_x, highest_y, highest_z, lowest_x, lowest_y, lowest_z;
            highest_x = 0;
            highest_y = 0;
            highest_z = 0;
            lowest_x = 0;
            lowest_y = 0;
            lowest_z = 0;

            if box.top_plane.center.x > highest_x
                highest_x = box.top_plane.center.x;
            if box.top_plane.center.y > highest_y
                highest_y = box.top_plane.center.y;
            if box.top_plane.center.z > highest_z
                highest_z = box.top_plane.center.z;
            if box.top_plane.center.x < lowest_x
                lowest_x = box.top_plane.center.x;
            if box.top_plane.center.y < lowest_y
                lowest_y = box.top_plane.center.y;
            if box.top_plane.center.z < lowest_z
                lowest_z = box.top_plane.center.z;
            if box.bottom_plane.center.x > highest_x
                highest_x = box.bottom_plane.center.x;
            if box.bottom_plane.center.y > highest_y
                highest_y = box.bottom_plane.center.y;
            if box.bottom_plane.center.z > highest_z
                highest_z = box.bottom_plane.center.z;
            if box.bottom_plane.center.x < lowest_x
                lowest_x = box.bottom_plane.center.x;
            if box.bottom_plane.center.y < lowest_y
                lowest_y = box.bottom_plane.center.y;
            if box.bottom_plane.center.z < lowest_z
                lowest_z = box.bottom_plane.center.z;
            if box.left_plane.center.x > highest_x
                highest_x = box.left_plane.center.x;
            if box.left_plane.center.y > highest_y
                highest_y = box.left_plane.center.y;
            if box.left_plane.center.z > highest_z
                highest_z = box.left_plane.center.z;
            if box.left_plane.center.x < lowest_x
                lowest_x = box.left_plane.center.x;
            if box.left_plane.center.y < lowest_y
                lowest_y = box.left_plane.center.y;
            if box.left_plane.center.z < lowest_z
                lowest_z = box.left_plane.center.z;
            if box.right_plane.center.x > highest_x
                highest_x = box.right_plane.center.x;
            if box.right_plane.center.y > highest_y
                highest_y = box.right_plane.center.y;
            if box.right_plane.center.z > highest_z
                highest_z = box.right_plane.center.z;
            if box.right_plane.center.x < lowest_x
                lowest_x = box.right_plane.center.x;
            if box.right_plane.center.y < lowest_y
                lowest_y = box.right_plane.center.y;
            if box.right_plane.center.z < lowest_z
                lowest_z = box.right_plane.center.z;
            if box.front_plane.center.x > highest_x
                highest_x = box.front_plane.center.x;
            if box.front_plane.center.y > highest_y
                highest_y = box.front_plane.center.y;
            if box.front_plane.center.z > highest_z
                highest_z = box.front_plane.center.z;
            if box.front_plane.center.x < lowest_x
                lowest_x = box.front_plane.center.x;
            if box.front_plane.center.y < lowest_y
                lowest_y = box.front_plane.center.y;
            if box.front_plane.center.z < lowest_z
                lowest_z = box.front_plane.center.z;
            if box.back_plane.center.x > highest_x
                highest_x = box.back_plane.center.x;
            if box.back_plane.center.y > highest_y
                highest_y = box.back_plane.center.y;
            if box.back_plane.center.z > highest_z
                highest_z = box.back_plane.center.z;
            if box.back_plane.center.x < lowest_x
                lowest_x = box.back_plane.center.x;
            if box.back_plane.center.y < lowest_y
                lowest_y = box.back_plane.center.y;
            if box.back_plane.center.z < lowest_z
                lowest_z = box.back_plane.center.z;
            
            double x_midpoint = (highest_x + lowest_x) / 2;
            double y_midpoint = (highest_y + lowest_y) / 2;
            double z_midpoint = (highest_z + lowest_z) / 2;
            box.center = Vector{x_midpoint, y_midpoint, z_midpoint};
            
            return box;
        }



        
};