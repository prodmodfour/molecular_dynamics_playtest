#pragma once
#include <vector>
#include <tuple>
#include "Vector.h"
#include "BoxCornerAtoms.h"

class StructureAnalyser
{
    public:
        StructureAnalyser();

        Vector calculate_center_of_mass(std::vector<Atom> atoms);
        BoxCornerAtoms find_box_corner_atoms(std::vector<Atom> atoms)
        {
            // Set initial values for the box corner atoms
            BoxCornerAtoms box_corner_atoms;
            box_corner_atoms.corner_1 = Vector{0, 0, 0};
            box_corner_atoms.corner_2 = Vector{0, 0, 0};
            box_corner_atoms.corner_3 = Vector{0, 0, 0};
            box_corner_atoms.corner_4 = Vector{0, 0, 0};
            box_corner_atoms.corner_5 = Vector{0, 0, 0};
            box_corner_atoms.corner_6 = Vector{0, 0, 0};
            box_corner_atoms.corner_7 = Vector{0, 0, 0};
            box_corner_atoms.corner_8 = Vector{0, 0, 0};

            for (auto atom : atoms)
            {
                if (atom.x < box_corner_atoms.corner_1.x)
                    box_corner_atoms.corner_1 = Vector{atom.x, atom.y, atom.z};
                if (atom.x > box_corner_atoms.corner_2.x)
                    box_corner_atoms.corner_2 = Vector{atom.x, atom.y, atom.z};
                if (atom.y < box_corner_atoms.corner_3.y)
                    box_corner_atoms.corner_3 = Vector{atom.x, atom.y, atom.z};
                if (atom.y > box_corner_atoms.corner_4.y)
                    box_corner_atoms.corner_4 = Vector{atom.x, atom.y, atom.z};
                if (atom.z < box_corner_atoms.corner_5.z)
                    box_corner_atoms.corner_5 = Vector{atom.x, atom.y, atom.z};
                if (atom.z > box_corner_atoms.corner_6.z)
                    box_corner_atoms.corner_6 = Vector{atom.x, atom.y, atom.z};
                if (atom.z > box_corner_atoms.corner_7.z)
                    box_corner_atoms.corner_7 = Vector{atom.x, atom.y, atom.z};
                if (atom.z < box_corner_atoms.corner_8.z)
                    box_corner_atoms.corner_8 = Vector{atom.x, atom.y, atom.z};
            }

            return box_corner_atoms;
        }

        Plane find_plane_from_box(BoxCornerAtoms box_corner_atoms, std::string plane_name)
        {

        }

        Vector calculate_center
};