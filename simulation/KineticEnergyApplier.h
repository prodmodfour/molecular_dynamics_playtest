#pragma once
#include "Atom.h"
#include "Vector.h"
#include <cmath>
#include <vector>

void apply_kinetic_energy(Atom &atom, double kinetic_energy, Vector direction)
{
    // Convert eV to J_per_moule
    kinetic_energy = kinetic_energy * 1.602176634e-19 * 6.02214076e23;

    // Calculate the speed
    double speed = sqrt(2 * kinetic_energy / atom.mass);

    // Calculate velocity components using the unit vector
    atom.vx = speed * direction.x;
    atom.vy = speed * direction.y;
    atom.vz = speed * direction.z;
}

void apply_kinetic_energy_to_atoms(std::vector<Atom> &atoms, double kinetic_energy, Vector direction)
{
    for (Atom &atom : atoms)
    {
        apply_kinetic_energy(atom, kinetic_energy, direction);
    }
}
