#pragma once
#include "../atoms/Atom.h"
#include "Config.h"
#include "../physics/TotalEnergy.h"
#include "../physics/physics_functions.h"
#include <vector>
#include "Timestep.h"
#include "../atoms/AtomPairLibrary.h"
#include "../ui/SharedData.h"
#include <mutex>
#include "simulation_data_mutex.h"
#include <iostream>

std::mutex simulation_data_mutex;

namespace simulation
{

simulation::Timestep simulate_timestep(simulation::Config config, std::vector<atoms::Atom> atoms, double time, atoms::AtomPairLibrary &atom_pair_library)
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

simulation::Timestep simulate_timestep(simulation::Timestep timestep, atoms::AtomPairLibrary &atom_pair_library)
{
    physics::TotalEnergy total_energy(0, 0);
    physics::evaluate_interactions(timestep.config, total_energy, timestep.atoms, atom_pair_library); // Calculate forces

    for (atoms::Atom &atom : timestep.atoms)
    {
        physics::calculate_motion(timestep.config.timestep_size, atom);
    }

    double total_kinetic_energy = 0;
    for (atoms::Atom &atom : timestep.atoms)
    {
        total_kinetic_energy += atom.kinetic_energy;
    }

    total_energy.kinetic = total_kinetic_energy;
    timestep.time += timestep.config.timestep_size;

    return simulation::Timestep(timestep.config, timestep.atoms, total_energy, timestep.time);
}


void run_simulation(SharedData* shared_data, std::vector<simulation::Timestep>* simulation_data)
{
    while (true)
    {
        if(shared_data->simulation_ended)
        {
            break;
        }

        // --------------------------Preprocessing---------------------------------
        // Lock the shared data mutex to access shared variables.
        std::unique_lock<std::mutex> lock(shared_data->mutex);
        // Check if the UI has edited any timesteps.
        if (shared_data->indexes_of_timesteps_edited_by_ui.size() > 0)
        {

            shared_data->changed_by_ui_since_last_loop = true;
            // Start with the assumption that the latest timestep is the smallest index.
            int smallest_index_edited_by_ui = simulation_data->size() - 1;

            // Iterate over the indices of timesteps edited by the UI.
            for (int index : shared_data->indexes_of_timesteps_edited_by_ui)
            {
                // If a smaller index is found, update smallest_index.
                if (index < smallest_index_edited_by_ui)
                {
                    smallest_index_edited_by_ui = index;
                }
            }

            // Set the index of the latest timestep to be simulated to the smallest edited index.
            shared_data->index_of_latest_timestep_simulated = smallest_index_edited_by_ui;
        }

        // Apply changes since the last iteration of this loop
        if (shared_data->changed_by_ui_since_last_loop)
        {

        }

        shared_data->changed_by_ui_since_last_loop = false;

        // If the simulation is ahead of the display plus the buffer, wait for UI to catch up.
        if (shared_data->index_of_latest_timestep_simulated >= shared_data->index_of_latest_timestep_displayed + shared_data->buffer_size)
        {
            continue;
        }

        // -----------------------Simulation starts here-------------------------------
        int index_of_timestep_to_simulate = shared_data->index_of_latest_timestep_simulated + 1;

        lock.unlock();

        std::unique_lock<std::mutex> lock2(simulation_data_mutex);
        simulation::Timestep input_timestep = simulation_data->at(index_of_timestep_to_simulate);
        lock2.unlock();

        simulation::Timestep output_timestep = simulation::simulate_timestep(input_timestep, shared_data->atom_pair_library);

        std::unique_lock<std::mutex> lock3(simulation_data_mutex);
        // Determine whether the vector is large enough to store the new timestep
        if (index_of_timestep_to_simulate >= simulation_data->size())
        {
            simulation_data->push_back(output_timestep);
        }
        else
        {
            simulation_data->at(index_of_timestep_to_simulate) = output_timestep;
        }
        lock3.unlock();
    }
}

} // namespace simulation