#include <thread>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "Type_atom.h"


typedef struct 
{
    int x, y, z;
} block_dimensions;

void generate_atom_row(std::vector<Type_atom>& atom_block, Type_atom first_atom, block_dimensions cubes_in, double atom_spacing, std::string atom_type);
void generate_atom_xy_plane(std::vector<Type_atom>& atom_block, block_dimensions cubes_in, double atom_spacing, std::string atom_type);
void generate_atom_xyz_space(std::vector<Type_atom>& atom_block, Type_atom first_atom, block_dimensions cubes_in, double atom_spacing, std::string atom_type);

void print_atoms(std::vector<Type_atom> atoms)
{
    // Print all atoms
    for (int i = 0; i < atoms.size(); i++)
    {
        printf("Index %d x %f y %f z %f\n", i, atoms[i].x, atoms[i].y, atoms[i].z);
    }
}

std::vector<Type_atom> generate_atom_block(int cubes_in_x, int cubes_in_y, int cubes_in_z)
{
    printf("Cubes in x %d \nCubes in y %d\nCubes in z %d\n", cubes_in_x, cubes_in_y, cubes_in_z);

    block_dimensions cubes_in;
    cubes_in.x = cubes_in_x;
    cubes_in.y = cubes_in_y;
    cubes_in.z = cubes_in_z;
    double atom_spacing = 3.61; //Angstroms

    std::vector<Type_atom> atom_block;

    Type_atom first_corner_atom;
    first_corner_atom.x = 0;
    first_corner_atom.y = 0;
    first_corner_atom.z = 0;
    generate_atom_xyz_space(atom_block, first_corner_atom, cubes_in, atom_spacing, "corner");


    Type_atom first_centre_atom;
    first_centre_atom.x = atom_spacing / 2;
    first_centre_atom.y = atom_spacing / 2;
    first_centre_atom.z = atom_spacing / 2;
    generate_atom_xyz_space(atom_block, first_corner_atom, cubes_in, atom_spacing, "centre");

    return atom_block;
}

void generate_atom_row(std::vector<Type_atom>& atom_block, Type_atom first_atom, block_dimensions cubes_in, double atom_spacing, std::string atom_type)
{
    int number_repeats;
    if(atom_type == "corner")
    {
        number_repeats = cubes_in.x + 1;
    }
    else if (atom_type == "centre")
    {
        number_repeats = cubes_in.x;
    }
    else
    {
        std::cout << "Incorrect atom type (" << atom_type << ") given for atom row" << std::endl;
        return;
    }

    Type_atom atom;
    atom.y = first_atom.y;
    atom.z = first_atom.z;
    for (int i = 0; i < number_repeats; i++)
    {
        atom.x = first_atom.x + (i * atom_spacing);
        atom_block.push_back(atom);
    }
}

void generate_atom_xy_plane(std::vector<Type_atom>& atom_block, Type_atom first_atom, block_dimensions cubes_in, double atom_spacing, std::string atom_type)
{
    int number_repeats;
    if(atom_type == "corner")
    {
        number_repeats = cubes_in.y + 1;
    }
    else if (atom_type == "centre")
    {
        number_repeats = cubes_in.y;
    }
    else
    {
        std::cout << "Incorrect atom type (" << atom_type << ") given for atom plane" << std::endl;
        return;
    }

    for (int i = 0; i < number_repeats; i++)
    {
        first_atom.y += (i * atom_spacing);
        generate_atom_row(atom_block, first_atom, cubes_in, atom_spacing, atom_type);
    }
}

void generate_atom_xyz_space(std::vector<Type_atom>& atom_block, Type_atom first_atom, block_dimensions cubes_in, double atom_spacing, std::string atom_type)
{
    int number_repeats;
    if(atom_type == "corner")
    {
        number_repeats = cubes_in.z + 1;
    }
    else if (atom_type == "centre")
    {
        number_repeats = cubes_in.z;
    }
    else
    {
        std::cout << "Incorrect atom type (" << atom_type << ") given for atom block" << std::endl;
        return;
    }

    for (int i = 0; i < number_repeats; i++)
    {
        first_atom.z += (i * atom_spacing);
        generate_atom_xy_plane(atom_block, first_atom, cubes_in, atom_spacing, atom_type);
    }

}