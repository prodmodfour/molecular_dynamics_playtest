#ifndef __generate_atoms_h
#define __generate_atoms_h

#include <thread>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#include "Atom.h"
#include "Settings.h"



typedef struct {
    int x_cubes, y_cubes, z_cubes;
    double atom_spacing;
} block_parameters;
void print_atoms(std::vector<Atom>& atoms);

void generate_atom_row(std::vector<Atom>& atom_block, Atom first_atom, block_parameters parameters,  std::string atom_type);
void generate_atom_xy_plane(std::vector<Atom>& atom_block, block_parameters parameters,  std::string atom_type);
void generate_atom_xyz_space(std::vector<Atom>& atom_block, Atom first_atom, block_parameters parameters,  std::string atom_type);
std::vector<Atom> generate_bcc(Settings settings);
std::vector<Atom> generate_fcc(Settings settings);

void add_impact_atom(std::vector<Atom>& atom_block, Settings settings);

void print_atoms(std::vector<Atom>& atoms)
{
    // Print all atoms
    for (int i = 0; i < atoms.size(); i++)
    {
        printf("Index %d x %f y %f z %f\n", i, atoms[i].x, atoms[i].y, atoms[i].z);
    }
}

void print_atoms_full(std::vector<Atom>& atoms)
{
    // Print all atoms
    for (int i = 0; i < atoms.size(); i++)
    {
        printf("Index %d x %f y %f z %f\n", i, atoms[i].x, atoms[i].y, atoms[i].z);
        printf("         fx %f fy %f fz %f\n", atoms[i].fx, atoms[i].fy, atoms[i].fz);
        printf("         vx %f vy %f vz %f\n", atoms[i].vx, atoms[i].vy, atoms[i].vz);
    }
}





std::vector<Atom> generate_bcc(Settings settings)
{
    std::vector<Atom> atom_block;
    block_parameters parameters;

    parameters.x_cubes  = settings.get_cubes_in_x();
    parameters.y_cubes = settings.get_cubes_in_y();
    parameters.z_cubes = settings.get_cubes_in_z();
    parameters.atom_spacing = settings.get_atom_spacing();

    Atom first_corner_atom;
    first_corner_atom.x = 0.0;
    first_corner_atom.y = 0.0;
    first_corner_atom.z = 0.0;
    generate_atom_xyz_space(atom_block, first_corner_atom, parameters, "corner");


    Atom first_centre_atom;

    first_centre_atom.x = parameters.atom_spacing / 2;
    first_centre_atom.y = parameters.atom_spacing / 2;
    first_centre_atom.z = parameters.atom_spacing / 2;
    generate_atom_xyz_space(atom_block, first_centre_atom, parameters, "centre");

    return atom_block;
}

void generate_atom_row(std::vector<Atom>& atom_block, Atom first_atom, block_parameters parameters, std::string atom_type)
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

    Atom atom;
    atom.y = first_atom.y;
    atom.z = first_atom.z;
    for (int i = 0; i < number_repeats; i++)
    {
        atom.x = first_atom.x + (i * parameters.atom_spacing);
        atom_block.push_back(atom);
    }
}

void generate_atom_xy_plane(std::vector<Atom>& atom_block, Atom first_atom, block_parameters parameters, std::string atom_type)
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

    

    Atom atom;
    atom.x = first_atom.x;
    atom.z = first_atom.z;
    for (int i = 0; i < number_repeats; i++)
    {
        atom.y = first_atom.y + (i * parameters.atom_spacing);
        generate_atom_row(atom_block, atom, parameters, atom_type);
    }
}

void generate_atom_xyz_space(std::vector<Atom>& atom_block, Atom first_atom, block_parameters parameters, std::string atom_type)
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


    Atom atom;
    atom.x = first_atom.x;
    atom.y = first_atom.y;
    
    for (int i = 0; i < number_repeats; i++)
    {
        atom.z = first_atom.z + (i * parameters.atom_spacing);
        generate_atom_xy_plane(atom_block, atom, parameters, atom_type);
    }
}

void add_impact_atom(std::vector<Atom>& atom_block, Settings settings)
{
    // Adds an impact atom z-offset above the centre of the top xy_surface of the block
    // This function assumes that the input atom block was generated by the other functions in this file. 
    Atom impact_atom;

    double y_offset = settings.get_impact_atom_y_offset();
    if (settings.get_atom_mode() == "generate")
    {   
        double x_cubes  = settings.get_cubes_in_x();
        double y_cubes = settings.get_cubes_in_y();
        double z_cubes = settings.get_cubes_in_z();
        double atom_spacing = settings.get_atom_spacing();
        


        double highest_x = x_cubes * atom_spacing;
        double x = (highest_x) / 2;
        impact_atom.x = x;

        double highest_y = y_cubes * atom_spacing;
        impact_atom.y = highest_y + y_offset;


        double highest_z = z_cubes * atom_spacing;
        double z = (highest_z / 2);
        impact_atom.z = z;
    }
    else if (settings.get_atom_mode() == "from_file")
    {
        double highest_x = 0;
        double highest_y = 0;
        double highest_z = 0;
        double lowest_x = 0;
        double lowest_y = 0;
        double lowest_z = 0;

        for (int i = 0; i < atom_block.size(); i++)
        {
            Atom atom = atom_block[i];

            if (highest_x < atom.x)
            {
                highest_x = atom.x;
            }
            if (highest_y < atom.y)
            {
                highest_y = atom.y;
            }
            if (highest_z < atom.z)
            {
                highest_z = atom.z;
            }

            if (lowest_x > atom.x)
            {
                lowest_x = atom.x;
            }
            if (lowest_y > atom.y)
            {
                lowest_y = atom.y;
            }
            if (lowest_z > atom.z)
            {
                lowest_z = atom.z;
            }

            impact_atom.x = (highest_x + lowest_x) / 2;
            impact_atom.y = highest_y + y_offset;
            impact_atom.z = (highest_z + lowest_z) / 2;

        }
    }


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

    impact_atom.reference_ke = 0.175;

    atom_block.push_back(impact_atom);
}

std::vector<Atom> generate_fcc(Settings settings)
{
    std::vector<Atom> crystal;

    int x_cubes  = settings.get_cubes_in_x();
    int y_cubes = settings.get_cubes_in_y();
    int z_cubes = settings.get_cubes_in_z();
    double atom_spacing = settings.get_atom_spacing();


    // FCC basis vectors
    std::vector<std::vector<double>> basis =
     {
        {0.0, 0.0, 0.0},
        {0.0, 0.5, 0.5},
        {0.5, 0.0, 0.5},
        {0.5, 0.5, 0.0}
    };

    // Generate crystal structure
    for (int ix = 0; ix < x_cubes; ix++) {
        for (int iy = 0; iy < y_cubes; iy++) {
            for (int iz = 0; iz < z_cubes; iz++) {
                for (const auto &b : basis) {
                    // Calculate atom coordinates based on unit cell and displacement
                    Atom atom;
                    atom.x = (ix + b[0]) * atom_spacing;
                    atom.y = (iy + b[1]) * atom_spacing;
                    atom.z = (iz + b[2]) * atom_spacing;
                    atom.reference_ke = 0.175;

                    // Add the atom to the crystal
                    crystal.push_back(atom);
                }
            }
        }
    }

    return crystal;
}

#endif