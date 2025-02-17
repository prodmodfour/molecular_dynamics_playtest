#pragma once
#include "../atoms/Atom.h"
#include "Config.h"
#include "../physics/TotalEnergy.h"
#include "../physics/physics_functions.h"
#include <vector>
#include "Timestep.h"
#include "../atoms/AtomPairLibrary.h"

namespace simulation
{

simulation::Timestep simulate_timestep(simulation::Config config, std::vector<atoms::Atom> atoms, double &time, atoms::AtomPairLibrary &atom_pair_library)
{
    physics::TotalEnergy total_energy(0, 0);
    physics::evaluate_interactions(config, total_energy, atoms, atom_pair_library); // Calculate forces

    for (atoms::Atom &atom : atoms)
    {
        physics::calculate_motion(config.timestep_size, atom);
    }

    double total_kinetic_energy = 0;
    for (atoms::Atom &atom : atoms)
    {
        total_kinetic_energy += atom.kinetic_energy;
    }

    total_energy.kinetic = total_kinetic_energy;
    time += config.timestep_size;

    return simulation::Timestep(config, atoms, total_energy, time);
}

} // namespace simulation