#ifndef __AtomGenerator_h
#define __AtomGenerator_h

#include <vector>
#include <string>
#include <random>
#include <limits>
#include <cmath>
#include "Atom.h"
#include "Settings.h"

class AtomGenerator {
public:
    // Constructor
    AtomGenerator(const Settings& settings);

    // Method to generate FCC lattice
    std::vector<Atom> generate_fcc();

    // Methods to add impact atoms
    void add_impact_atom(std::vector<Atom>& atom_block, const std::string& surface = "top");
    void add_random_impact_atom(std::vector<Atom>& atom_block, const std::string& surface = "top");
    void add_impact_atom_random_surface(std::vector<Atom>& atom_block);

private:
    // Helper methods
    void update_bounding_box(const Atom& atom);
    void calculate_bounding_box_center();

    // Member variables
    Settings settings;
    double reference_ke;

    // Bounding box variables
    double x_min, x_max;
    double y_min, y_max;
    double z_min, z_max;
    double x_center, y_center, z_center;
};

//// Implementation

// Constructor
AtomGenerator::AtomGenerator(const Settings& settings)
    : settings(settings), reference_ke(0.02)
{
    // Initialize bounding box variables
    x_min = y_min = z_min = std::numeric_limits<double>::max();
    x_max = y_max = z_max = std::numeric_limits<double>::lowest();
    x_center = y_center = z_center = 0.0;
}

// Helper method to update bounding box
void AtomGenerator::update_bounding_box(const Atom& atom)
{
    if (atom.x < x_min) x_min = atom.x;
    if (atom.x > x_max) x_max = atom.x;
    if (atom.y < y_min) y_min = atom.y;
    if (atom.y > y_max) y_max = atom.y;
    if (atom.z < z_min) z_min = atom.z;
    if (atom.z > z_max) z_max = atom.z;
}

// Helper method to calculate the center of the bounding box
void AtomGenerator::calculate_bounding_box_center()
{
    x_center = (x_min + x_max) / 2.0;
    y_center = (y_min + y_max) / 2.0;
    z_center = (z_min + z_max) / 2.0;
}

// Method to generate FCC lattice and calculate bounding box
std::vector<Atom> AtomGenerator::generate_fcc()
{
    std::vector<Atom> crystal;

    int x_cubes = settings.get_cubes_in_x();
    int y_cubes = settings.get_cubes_in_y();
    int z_cubes = settings.get_cubes_in_z();
    double atom_spacing = settings.get_atom_spacing();

    // FCC basis vectors
    std::vector<std::vector<double>> basis = {
        {0.0, 0.0, 0.0},
        {0.0, 0.5, 0.5},
        {0.5, 0.0, 0.5},
        {0.5, 0.5, 0.0}
    };

    // Generate crystal structure
    for (int ix = 0; ix < x_cubes; ix++) {
        for (int iy = 0; iy < y_cubes; iy++) {
            for (int iz = 0; iz < z_cubes; iz++) {
                for (const auto& b : basis) {
                    Atom atom;
                    atom.x = (ix + b[0]) * atom_spacing;
                    atom.y = (iy + b[1]) * atom_spacing;
                    atom.z = (iz + b[2]) * atom_spacing;
                    atom.reference_ke = reference_ke;

                    // Initialize velocities and forces
                    atom.vx = atom.vy = atom.vz = 0.0;
                    atom.fx = atom.fy = atom.fz = 0.0;

                    // Update bounding box
                    update_bounding_box(atom);

                    // Add atom to crystal
                    crystal.push_back(atom);
                }
            }
        }
    }

    // Calculate center of the bounding box
    calculate_bounding_box_center();

    return crystal;
}

// Method to add an impact atom from a specified surface
void AtomGenerator::add_impact_atom(std::vector<Atom>& atom_block, const std::string& surface)
{
    Atom impact_atom;

    double x_offset = settings.get_impact_atom_x_offset();
    double y_offset = settings.get_impact_atom_y_offset();
    double z_offset = settings.get_impact_atom_z_offset();

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

// Method to add an impact atom aimed at a random spot on a specified surface
void AtomGenerator::add_random_impact_atom(std::vector<Atom>& atom_block, const std::string& surface)
{
    Atom impact_atom;

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

    // Set the starting position of the impact atom outside the block
    double x_offset = settings.get_impact_atom_x_offset();
    double y_offset = settings.get_impact_atom_y_offset();
    double z_offset = settings.get_impact_atom_z_offset();

    if (surface == "top")
    {
        impact_atom.x = target_x + x_offset;
        impact_atom.y = y_max + y_offset;
        impact_atom.z = target_z + z_offset;
    }
    else if (surface == "bottom")
    {
        impact_atom.x = target_x + x_offset;
        impact_atom.y = y_min - y_offset;
        impact_atom.z = target_z + z_offset;
    }
    else if (surface == "left")
    {
        impact_atom.x = x_min - x_offset;
        impact_atom.y = target_y + y_offset;
        impact_atom.z = target_z + z_offset;
    }
    else if (surface == "right")
    {
        impact_atom.x = x_max + x_offset;
        impact_atom.y = target_y + y_offset;
        impact_atom.z = target_z + z_offset;
    }
    else if (surface == "front")
    {
        impact_atom.x = target_x + x_offset;
        impact_atom.y = target_y + y_offset;
        impact_atom.z = z_min - z_offset;
    }
    else if (surface == "back")
    {
        impact_atom.x = target_x + x_offset;
        impact_atom.y = target_y + y_offset;
        impact_atom.z = z_max + z_offset;
    }
    else
    {
        // Default to top surface
        impact_atom.x = target_x + x_offset;
        impact_atom.y = y_max + y_offset;
        impact_atom.z = target_z + z_offset;
    }

    // Compute the direction vector towards the center of the block
    double dir_x = x_center - impact_atom.x;
    double dir_y = y_center - impact_atom.y;
    double dir_z = z_center - impact_atom.z;

    // Normalize the direction vector
    double dir_length = std::sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
    dir_x /= dir_length;
    dir_y /= dir_length;
    dir_z /= dir_length;

    // Compute the velocity magnitude based on the applied energy
    double ev_to_j_per_mole = settings.get_ev_to_j_per_mole();
    double atom_mass = settings.get_atom_mass();
    double applied_energy = settings.get_energy_applied_to_impact_atom() * ev_to_j_per_mole;

    double velocity_magnitude = std::sqrt((2.0 * applied_energy) / atom_mass);

    // Set the velocities in the direction of the target point
    impact_atom.vx = dir_x * velocity_magnitude;
    impact_atom.vy = dir_y * velocity_magnitude;
    impact_atom.vz = dir_z * velocity_magnitude;

    // Initialize forces
    impact_atom.fx = 0.0;
    impact_atom.fy = 0.0;
    impact_atom.fz = 0.0;

    // Set reference kinetic energy
    impact_atom.reference_ke = reference_ke;

    // Add the impact atom to the atom block
    atom_block.push_back(impact_atom);
}

// Method to add an impact atom from a random surface
void AtomGenerator::add_impact_atom_random_surface(std::vector<Atom>& atom_block)
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
    add_random_impact_atom(atom_block, random_surface);
}

#endif
