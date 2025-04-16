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


void run_simulation(ui::SharedData* shared_data, simulation::Timestep* simulation_data, atoms::AtomPairLibrary &atom_pair_library)
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(shared_data->mutex);
        if (shared_data->indexes_of_timesteps_edited_by_ui.size() > 0)
        {
            shared_data->changed_by_ui_since_last_loop = true;
            int smallest_index = shared_data->simulation_data.size() - 1;

            for (int index : shared_data->indexes_of_timesteps_edited_by_ui)
            {
                if (index < smallest_index)
                {
                    smallest_index = index;
                }
            }

            shared_data->index_of_latest_timestep_simulated = smallest_index;
            
            
        }

        if (shared_data->changed_by_ui_since_last_loop)
        {

        }
        shared_data->changed_by_ui_since_last_loop = false;

        if (shared_data->index_of_latest_timestep_simulated >= shared_data->index_of_latest_timestep_displayed + shared_data->buffer_size)
        {
            continue;
        }

        int index_of_timestep_to_simulate = shared_data->index_of_latest_timestep_simulated + 1;
        lock.unlock();

        std::unique_lock<std::mutex> lock2(simulation_data_mutex);
        simulation::Timestep timestep = simulation_data[index_of_timestep_to_simulate];
        lock2.unlock();

        simulation::Timestep timestep = simulation::simulate_timestep(timestep, atom_pair_library);

        std::unique_lock<std::mutex> lock3(simulation_data_mutex);
        if (index_of_timestep_to_simulate >= simulation_data->size())
        {
            simulation_data->push_back(timestep);
        }
        else
        {
            simulation_data[index_of_timestep_to_simulate] = timestep;
        }
        lock3.unlock();


    }
}

} // namespace simulation