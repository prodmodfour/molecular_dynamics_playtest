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
#include <random>

double reference_ke = 0.02;

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

void add_impact_atom(std::vector<Atom>& atom_block, const Settings& settings, const std::string& surface = "top")
{
    // Adds an impact atom offset from the specified surface of the block
    Atom impact_atom;

    double x_offset = settings.get_impact_atom_x_offset();
    double y_offset = settings.get_impact_atom_y_offset();
    double z_offset = settings.get_impact_atom_z_offset();


    // Initialize min and max values
    double x_min = std::numeric_limits<double>::max();
    double x_max = std::numeric_limits<double>::lowest();
    double y_min = std::numeric_limits<double>::max();
    double y_max = std::numeric_limits<double>::lowest();
    double z_min = std::numeric_limits<double>::max();
    double z_max = std::numeric_limits<double>::lowest();

    // Find the bounding box of the atom block
    for (const Atom& atom : atom_block)
    {
        if (atom.x < x_min) x_min = atom.x;
        if (atom.x > x_max) x_max = atom.x;
        if (atom.y < y_min) y_min = atom.y;
        if (atom.y > y_max) y_max = atom.y;
        if (atom.z < z_min) z_min = atom.z;
        if (atom.z > z_max) z_max = atom.z;
    }

    // Calculate the center of the block
    double x_center = (x_min + x_max) / 2.0;
    double y_center = (y_min + y_max) / 2.0;
    double z_center = (z_min + z_max) / 2.0;

    // Set the starting position of the impact atom based on the specified surface
    if (surface == "top")
    {
        impact_atom.x = x_center + x_offset;
        impact_atom.y = y_max + y_offset;  // Above the top surface
        impact_atom.z = z_center + z_offset;
    }
    else if (surface == "bottom")
    {
        impact_atom.x = x_center + x_offset;
        impact_atom.y = y_min - y_offset;  // Below the bottom surface
        impact_atom.z = z_center + z_offset;
    }
    else if (surface == "left")
    {
        impact_atom.x = x_min - x_offset;  // Left of the left surface
        impact_atom.y = y_center + y_offset;
        impact_atom.z = z_center + z_offset;
    }
    else if (surface == "right")
    {
        impact_atom.x = x_max + x_offset;  // Right of the right surface
        impact_atom.y = y_center + y_offset;
        impact_atom.z = z_center + z_offset;
    }
    else if (surface == "front")
    {
        impact_atom.x = x_center + x_offset;
        impact_atom.y = y_center + y_offset;
        impact_atom.z = z_min - z_offset;  // In front of the front surface
    }
    else if (surface == "back")
    {
        impact_atom.x = x_center + x_offset;
        impact_atom.y = y_center + y_offset;
        impact_atom.z = z_max + z_offset;  // Behind the back surface
    }
    else
    {
        // Default to top surface if an unknown surface is specified
        impact_atom.x = x_center + x_offset;
        impact_atom.y = y_max + y_offset;
        impact_atom.z = z_center + z_offset;
    }

    // Calculate velocities based on energy shares
    double ev_to_j_per_mole = settings.get_ev_to_j_per_mole();
    double atom_mass = settings.get_atom_mass();
    double applied_energy = settings.get_energy_applied_to_impact_atom() * ev_to_j_per_mole;

    double x_energy_share = settings.get_impact_atom_x_energy_share();
    double y_energy_share = settings.get_impact_atom_y_energy_share();
    double z_energy_share = settings.get_impact_atom_z_energy_share();

    double x_applied_energy = applied_energy * x_energy_share;
    double y_applied_energy = applied_energy * y_energy_share;
    double z_applied_energy = applied_energy * z_energy_share;

    // Calculate velocities in each direction
    double vx = (x_applied_energy > 0) ? std::sqrt((2.0 * x_applied_energy) / atom_mass) : 0.0;
    double vy = (y_applied_energy > 0) ? std::sqrt((2.0 * y_applied_energy) / atom_mass) : 0.0;
    double vz = (z_applied_energy > 0) ? std::sqrt((2.0 * z_applied_energy) / atom_mass) : 0.0;

    // Set velocities towards the block based on the surface
    if (surface == "top")
    {
        impact_atom.vx = vx;
        impact_atom.vy = -vy;  // Moving downwards
        impact_atom.vz = vz;
    }
    else if (surface == "bottom")
    {
        impact_atom.vx = vx;
        impact_atom.vy = vy;   // Moving upwards
        impact_atom.vz = vz;
    }
    else if (surface == "left")
    {
        impact_atom.vx = vx;   // Moving right
        impact_atom.vy = vy;
        impact_atom.vz = vz;
    }
    else if (surface == "right")
    {
        impact_atom.vx = -vx;  // Moving left
        impact_atom.vy = vy;
        impact_atom.vz = vz;
    }
    else if (surface == "front")
    {
        impact_atom.vx = vx;
        impact_atom.vy = vy;
        impact_atom.vz = vz;   // Moving into the block
    }
    else if (surface == "back")
    {
        impact_atom.vx = vx;
        impact_atom.vy = vy;
        impact_atom.vz = -vz;  // Moving towards the front
    }
    else
    {
        // Default to top surface velocities
        impact_atom.vx = vx;
        impact_atom.vy = -vy;
        impact_atom.vz = vz;
    }

    // Initialize forces
    impact_atom.fx = 0.0;
    impact_atom.fy = 0.0;
    impact_atom.fz = 0.0;

    // Set reference kinetic energy 
    impact_atom.reference_ke = reference_ke;  

    // Add the impact atom to the atom block
    atom_block.push_back(impact_atom);
}

void add_random_impact_atom(std::vector<Atom>& atom_block, const Settings& settings, const std::string& surface = "top")
{
    // Adds an impact atom aimed at a random spot on the specified surface of the atom block
    // surface can be "top", "bottom", "left", "right", "front", "back"
    Atom impact_atom;

    double x_min = std::numeric_limits<double>::max();
    double x_max = std::numeric_limits<double>::lowest();
    double y_min = std::numeric_limits<double>::max();
    double y_max = std::numeric_limits<double>::lowest();
    double z_min = std::numeric_limits<double>::max();
    double z_max = std::numeric_limits<double>::lowest();

    // Find the bounding box of the atom block
    for (const Atom& atom : atom_block)
    {
        if (atom.x < x_min) x_min = atom.x;
        if (atom.x > x_max) x_max = atom.x;
        if (atom.y < y_min) y_min = atom.y;
        if (atom.y > y_max) y_max = atom.y;
        if (atom.z < z_min) z_min = atom.z;
        if (atom.z > z_max) z_max = atom.z;
    }

    // Random number generators
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> x_dis(x_min, x_max);
    std::uniform_real_distribution<> y_dis(y_min, y_max);
    std::uniform_real_distribution<> z_dis(z_min, z_max);

    // Select a random point on the specified surface
    double target_x, target_y, target_z;
    if (surface == "top")
    {
        target_x = x_dis(gen);
        target_y = y_max;  // Top surface (maximum y)
        target_z = z_dis(gen);
    }
    else if (surface == "bottom")
    {
        target_x = x_dis(gen);
        target_y = y_min;  // Bottom surface (minimum y)
        target_z = z_dis(gen);
    }
    else if (surface == "left")
    {
        target_x = x_min;  // Left surface (minimum x)
        target_y = y_dis(gen);
        target_z = z_dis(gen);
    }
    else if (surface == "right")
    {
        target_x = x_max;  // Right surface (maximum x)
        target_y = y_dis(gen);
        target_z = z_dis(gen);
    }
    else if (surface == "front")
    {
        target_x = x_dis(gen);
        target_y = y_dis(gen);
        target_z = z_min;  // Front surface (minimum z)
    }
    else if (surface == "back")
    {
        target_x = x_dis(gen);
        target_y = y_dis(gen);
        target_z = z_max;  // Back surface (maximum z)
    }
    else
    {
        // Default to top surface if an unknown surface is specified
        target_x = x_dis(gen);
        target_y = y_max;
        target_z = z_dis(gen);
    }

    // Set the starting position of the impact atom (above the top surface)
    double y_offset = settings.get_impact_atom_y_offset();
    impact_atom.x = (x_min + x_max) / 2.0;
    impact_atom.y = y_max + y_offset;
    impact_atom.z = (z_min + z_max) / 2.0;

    // Compute the direction vector towards the target point
    double dir_x = target_x - impact_atom.x;
    double dir_y = target_y - impact_atom.y;
    double dir_z = target_z - impact_atom.z;

    // Normalize the direction vector
    double dir_length = std::sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
    dir_x /= dir_length;
    dir_y /= dir_length;
    dir_z /= dir_length;

    // Compute the velocity magnitude based on the applied energy
    double ev_to_j_per_mole = settings.get_ev_to_j_per_mole();
    double atom_mass = settings.get_atom_mass();
    double applied_energy = settings.get_energy_applied_to_impact_atom();
    applied_energy *= ev_to_j_per_mole;  // Convert energy to J/mol

    double velocity_magnitude = std::sqrt((2.0 * applied_energy) / atom_mass);

    // Set the velocities in the direction of the target point
    impact_atom.vx = dir_x * velocity_magnitude;
    impact_atom.vy = dir_y * velocity_magnitude;
    impact_atom.vz = dir_z * velocity_magnitude;

    // Initialize forces and other properties
    impact_atom.fx = 0;
    impact_atom.fy = 0;
    impact_atom.fz = 0;
    impact_atom.reference_ke = reference_ke;  // Ensure you have this variable defined

    // Add the impact atom to the atom block
    atom_block.push_back(impact_atom);
}

void add_impact_atom_random_surface(std::vector<Atom>& atom_block, const Settings& settings)
{
    // List of possible surfaces
    std::vector<std::string> surfaces = {"top", "bottom", "left", "right", "front", "back"};

    // Random number generator setup
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> surface_dis(0, surfaces.size() - 1);

    // Randomly select a surface
    std::string random_surface = surfaces[surface_dis(gen)];

    // Call the existing function with the randomly selected surface
    add_random_impact_atom(atom_block, settings, random_surface);
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
                    atom.reference_ke = reference_ke;

                    // Add the atom to the crystal
                    crystal.push_back(atom);
                }
            }
        }
    }

    return crystal;
}

#endif