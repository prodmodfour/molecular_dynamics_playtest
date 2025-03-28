#pragma once
#include "../atoms/AtomPairLibrary.h"
#include "../atoms/Atom.h"
#include <vector>
#include "../simulation/Config.h"
#include "TotalEnergy.h"
#include <cmath>
#include "../atoms/AtomPairData.h"

namespace physics
{

void zero_forces(std::vector<atoms::Atom> &atoms);

void evaluate_interactions(simulation::Config config, physics::TotalEnergy &total_energy, std::vector<atoms::Atom> &atoms, atoms::AtomPairLibrary atom_pair_library)
{
    zero_forces(atoms);

    double xi, yi, zi;
    double xj, yj, zj;
    double dx, dy, dz;
    double fxij, fyij, fzij, fij;
    double squared_distance, distance, sigma_squared;
    double sr2, sr6, sr12, epsilon4, epsilon24;
    double potential_ij, unit_vector_x, unit_vector_y, unit_vector_z;
    double reciprocal_distance;

    total_energy.potential = 0;

    double r_cutoff = config.r_cutoff;
    double r_cutoff_squared = r_cutoff * r_cutoff;

    for (int i = 0; i < atoms.size(); i++)
    {
        atoms::Atom &atom_i = atoms[i];
        xi = atom_i.x;
        yi = atom_i.y;
        zi = atom_i.z;

        for (int j = i + 1; j < atoms.size(); j++)
        {
            atoms::Atom &atom_j = atoms[j];
            xj = atom_j.x;
            yj = atom_j.y;
            zj = atom_j.z;

            dx = xi - xj;
            if (dx > r_cutoff)
            {
                continue;
            }
            dy = yi - yj;
            if (dy > r_cutoff)
            {
                continue;
            }
            dz = zi - zj;
            if (dz > r_cutoff)
            {
                continue;
            }

            squared_distance = dx*dx + dy*dy + dz*dz;
            if (squared_distance > r_cutoff_squared)
            {
                continue;
            }

            distance = std::sqrt(squared_distance);

            atoms::AtomPairData atom_pair_data = atom_pair_library.get_atom_pair_data(atom_i.type, atom_j.type);
            double sigma = atom_pair_data.sigma;
            double epsilon = atom_pair_data.epsilon;

            sigma_squared = sigma * sigma;
            sr2 = sigma_squared / squared_distance;
            sr6 = sr2 * sr2 * sr2;
            sr12 = sr6 * sr6;
            epsilon4 = 4 * epsilon;
            epsilon24 = 24 * epsilon;

            reciprocal_distance = 1 / distance;
            unit_vector_x = dx * reciprocal_distance;
            unit_vector_y = dy * reciprocal_distance;
            unit_vector_z = dz * reciprocal_distance;

            potential_ij = epsilon4 * (sr12 - sr6);
            total_energy.potential += potential_ij;

            fij = epsilon24 * (2 * sr12 - sr6) * reciprocal_distance;
            fxij = fij * unit_vector_x;
            fyij = fij * unit_vector_y;
            fzij = fij * unit_vector_z;

            /* Sum the forces on i due to all j */
            atoms[i].fx += fxij;
            atoms[i].fy += fyij;
            atoms[i].fz += fzij;

            /* Sum the forces on j due to i in ij direction */
            atoms[j].fx -= fxij;
            atoms[j].fy -= fyij;
            atoms[j].fz -= fzij;
        }


    }
    // Convert to eV
    total_energy.potential *= 1.602176634e-19;
}

void zero_forces(std::vector<atoms::Atom> &atoms)
{
    for (atoms::Atom &atom : atoms)
    {
        atom.fx = 0.0;
        atom.fy = 0.0;
        atom.fz = 0.0;
    }
}

void apply_kinetic_energy(atoms::Atom &atom, double kinetic_energy, geometry::Vector direction)
{
    // Convert eV to J_per_moule
    kinetic_energy = kinetic_energy * 1.602176634e-19 * 6.02214076e23;

    // Calculate the speed
    double speed = sqrt(2 * kinetic_energy / atom.mass);

    // Calculate velocity components using the unit vector
    atom.vx = speed * direction.x;
    atom.vy = speed * direction.y;
    atom.vz = speed * direction.z;

    // Set the kinetic energy
    atom.kinetic_energy += kinetic_energy;
}

void apply_kinetic_energy_to_atoms(std::vector<atoms::Atom> &atoms, double kinetic_energy, geometry::Vector direction)
{
    for (atoms::Atom &atom : atoms)
    {
        physics::apply_kinetic_energy(atom, kinetic_energy, direction);
    }
}

void calculate_motion(double timestep_size, std::vector<atoms::Atom> &atoms)
{
    double v_squared;
    double vxi, vyi, vzi;
    double vxi2, vyi2, vzi2;
    double vxi3, vyi3, vzi3;
    double fxi, fyi, fzi;
    double delta_vxi, delta_vyi, delta_vzi;
    double scaling_factor = 0.01;

    for (atoms::Atom &atom : atoms)
    {
        double velocity_integration_factor = scaling_factor * timestep_size / atom.mass;

        // Calculate velocity V(t + 0.5dt)
        vxi = atom.vx;
        vyi = atom.vy;
        vzi = atom.vz;

        fxi = atom.fx;
        fyi = atom.fy;
        fzi = atom.fz;

        delta_vxi = fxi * velocity_integration_factor;
        delta_vyi = fyi * velocity_integration_factor;
        delta_vzi = fzi * velocity_integration_factor;

        vxi2 = vxi + delta_vxi;
        vyi2 = vyi + delta_vyi;
        vzi2 = vzi + delta_vzi;

        // Update positions
        atom.x += vxi2 * timestep_size;
        atom.y += vyi2 * timestep_size;
        atom.z += vzi2 * timestep_size;

        // Calculate actual velocity at time t
        // For kinetic energy calculations only
        vxi3 = (vxi + vxi2) / 2;
        vyi3 = (vyi + vyi2) / 2;
        vzi3 = (vzi + vzi2) / 2;
        v_squared = vxi3*vxi3 + vyi3*vyi3 + vzi3*vzi3;
        atom.kinetic_energy = 0.5 * atom.mass * v_squared;
        // Convert to eV
        atom.kinetic_energy *= 1.602176634e-19;

        // Update velocities
        atom.vx = vxi2;
        atom.vy = vyi2;
        atom.vz = vzi2;
    }
}

void calculate_motion(double timestep_size, atoms::Atom &atom)
{
    double v_squared;
    double vxi, vyi, vzi;
    double vxi2, vyi2, vzi2;
    double vxi3, vyi3, vzi3;
    double fxi, fyi, fzi;

    double delta_vxi, delta_vyi, delta_vzi;
    double scaling_factor = 0.01;

    double velocity_integration_factor = scaling_factor * timestep_size / atom.mass;

    // Calculate velocity V(t + 0.5dt)
    vxi = atom.vx;
    vyi = atom.vy;
    vzi = atom.vz;

    fxi = atom.fx;
    fyi = atom.fy;
    fzi = atom.fz;

    delta_vxi = fxi * velocity_integration_factor;
    delta_vyi = fyi * velocity_integration_factor;
    delta_vzi = fzi * velocity_integration_factor;

    vxi2 = vxi + delta_vxi;
    vyi2 = vyi + delta_vyi;
    vzi2 = vzi + delta_vzi;

    // Update positions
    atom.x += vxi2 * timestep_size;
    atom.y += vyi2 * timestep_size;
    atom.z += vzi2 * timestep_size;

    // Calculate actual velocity at time t
    // For kinetic energy calculations only
    vxi3 = (vxi + vxi2) / 2;
    vyi3 = (vyi + vyi2) / 2;
    vzi3 = (vzi + vzi2) / 2;
    v_squared = vxi3*vxi3 + vyi3*vyi3 + vzi3*vzi3;
    atom.kinetic_energy = 0.5 * atom.mass * v_squared;
    // Convert to eV
    atom.kinetic_energy *= 1.602176634e-19;

    // Update velocities
    atom.vx = vxi2;
    atom.vy = vyi2;
    atom.vz = vzi2;
}
}