#ifndef __generate_atoms_h
#define __generate_atoms_h

#include <thread>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#include "Type_atom.h"
#include "Settings.h"



typedef struct {
    int x_cubes, y_cubes, z_cubes;
    double atom_spacing;
} block_parameters;
void print_atoms(std::vector<Type_atom>& atoms);

void generate_atom_row(std::vector<Type_atom>& atom_block, Type_atom first_atom, block_parameters parameters,  std::string atom_type);
void generate_atom_xy_plane(std::vector<Type_atom>& atom_block, block_parameters parameters,  std::string atom_type);
void generate_atom_xyz_space(std::vector<Type_atom>& atom_block, Type_atom first_atom, block_parameters parameters,  std::string atom_type);
std::vector<Type_atom> generate_atom_block(block_parameters parameters);

void add_impact_atom(std::vector<Type_atom>& atom_block, Settings settings);

void print_atoms(std::vector<Type_atom>& atoms)
{
    // Print all atoms
    for (int i = 0; i < atoms.size(); i++)
    {
        printf("Index %d x %f y %f z %f\n", i, atoms[i].x, atoms[i].y, atoms[i].z);
    }
}



std::vector<Type_atom> generate_atom_block(Settings settings)
{
    std::vector<Type_atom> atom_block;
    block_parameters parameters;

    parameters.x_cubes  = settings.get_cubes_in_x();
    parameters.y_cubes = settings.get_cubes_in_y();
    parameters.z_cubes = settings.get_cubes_in_z();
    parameters.atom_spacing = settings.get_atom_spacing();

    Type_atom first_corner_atom;
    first_corner_atom.x = 0;
    first_corner_atom.y = 0;
    first_corner_atom.z = 0;
    generate_atom_xyz_space(atom_block, first_corner_atom, parameters, "corner");


    Type_atom first_centre_atom;

    first_centre_atom.x = parameters.atom_spacing / 2;
    first_centre_atom.y = parameters.atom_spacing / 2;
    first_centre_atom.z = parameters.atom_spacing / 2;
    generate_atom_xyz_space(atom_block, first_centre_atom, parameters, "centre");

    return atom_block;
}

void generate_atom_row(std::vector<Type_atom>& atom_block, Type_atom first_atom, block_parameters parameters, std::string atom_type)
{
    int number_repeats;
    if(atom_type == "corner")
    {
        number_repeats = parameters.x_cubes + 1;
    }
    else if (atom_type == "centre")
    {
        number_repeats = parameters.x_cubes;
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
        atom.x = first_atom.x + (i * parameters.atom_spacing);
        atom_block.push_back(atom);
    }
}

void generate_atom_xy_plane(std::vector<Type_atom>& atom_block, Type_atom first_atom, block_parameters parameters, std::string atom_type)
{
    int number_repeats;
    if(atom_type == "corner")
    {
        number_repeats = parameters.y_cubes + 1;
    }
    else if (atom_type == "centre")
    {
        number_repeats = parameters.y_cubes;
    }
    else
    {
        std::cout << "Incorrect atom type (" << atom_type << ") given for atom plane" << std::endl;
        return;
    }

    

    Type_atom atom;
    atom.x = first_atom.x;
    atom.z = first_atom.z;
    for (int i = 0; i < number_repeats; i++)
    {
        atom.y = first_atom.y + (i * parameters.atom_spacing);
        generate_atom_row(atom_block, atom, parameters, atom_type);
    }
}

void generate_atom_xyz_space(std::vector<Type_atom>& atom_block, Type_atom first_atom, block_parameters parameters, std::string atom_type)
{
    int number_repeats;
    if(atom_type == "corner")
    {
        number_repeats = parameters.z_cubes + 1;
    }
    else if (atom_type == "centre")
    {
        number_repeats = parameters.z_cubes;
    }
    else
    {
        std::cout << "Incorrect atom type (" << atom_type << ") given for atom block" << std::endl;
        return;
    }


    Type_atom atom;
    atom.x = first_atom.x;
    atom.y = first_atom.y;
    
    for (int i = 0; i < number_repeats; i++)
    {
        atom.z = first_atom.z + (i * parameters.atom_spacing);
        generate_atom_xy_plane(atom_block, atom, parameters, atom_type);
    }
}

void add_impact_atom(std::vector<Type_atom>& atom_block, Settings settings)
{
    // Adds an impact atom z-offset above the centre of the top xy_surface of the block
    // This function assumes that the input atom block was generated by the other functions in this file. 
    Type_atom impact_atom;

    double x_cubes  = settings.get_cubes_in_x();
    double y_cubes = settings.get_cubes_in_y();
    double z_cubes = settings.get_cubes_in_z();
    double atom_spacing = settings.get_atom_spacing();
    double y_offset = settings.get_impact_atom_y_offset();


    double highest_x = x_cubes * atom_spacing;
    double x = (highest_x) / 2;
    impact_atom.x = x + 0.5;

    double highest_y = y_cubes * atom_spacing;
    impact_atom.y = highest_y + y_offset;


    double highest_z = z_cubes * atom_spacing;
    double z = (highest_z / 2);
    impact_atom.z = z;


    impact_atom.fx = 0;
    impact_atom.fy = 0;
    impact_atom.fz = 0;

    impact_atom.vx = 0;
    impact_atom.vz = 0;
    double ev_to_j_per_mole = settings.get_ev_to_j_per_mole();
    double atom_mass = settings.get_atom_mass();
    double applied_energy = settings.get_energy_applied_to_impact_atom();
    applied_energy *= ev_to_j_per_mole;
    
    impact_atom.vy = -std::sqrt((2.0*applied_energy)/atom_mass);

    atom_block.push_back(impact_atom);
}

#endif