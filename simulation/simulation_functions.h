#pragma once
#include "../atoms/Atom.h"
#include "Config.h"
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



simulation::Timestep simulate_timestep(simulation::Timestep input_timestep, atoms::AtomPairLibrary &atom_pair_library, SharedData* shared_data)
{
    double total_potential_energy = 0;
    simulation::Config config = shared_data->config;
    physics::evaluate_interactions(config, total_potential_energy, input_timestep.atoms, atom_pair_library); // Calculate forces

    for (atoms::Atom &atom : input_timestep.atoms)
    {
        physics::calculate_motion(config.timestep_size, atom);
    }

    double total_kinetic_energy = 0;
    for (atoms::Atom &atom : input_timestep.atoms)
    {
        total_kinetic_energy += atom.kinetic_energy;
    }

    input_timestep.time += config.timestep_size;

    simulation::Timestep output_timestep = simulation::Timestep(config, input_timestep.atoms, total_kinetic_energy, total_potential_energy, input_timestep.time);
    output_timestep.structure_list = input_timestep.structure_list;
    output_timestep.structures = input_timestep.structures;

    return output_timestep;
}


void run_simulation(SharedData* shared_data, std::vector<simulation::Timestep>* simulation_data)
{
    while (true)
    {
        if(shared_data->simulation_ended)
        {
            break;
        }

        // -------------------------------------Preprocess Section---------------------------------
        
 
        // There were two options that I considered for managing the data after the user has edited the simulation parameters mid simulation.
        // (This is important because, once a timestep is edited, every subsequent simulated timestep becomes obsolete and must be resimulated)

        // Option 1: Clear every timestep past the edited timestep and then start simulating again from the edited timestep.

        // Option 2: Keep track index_of_latest_timestep_simulated. Once the user edits a timestep, this variable becomes the index of the
        // edited timestep. The simulation then goes back to that timestep without clearing the obsolete data

        // I chose option 2 in order to save the time that it would otherwise take to clear the obsolete data
        // (For large buffer sizes and large systems, this could be considerable)



        std::unique_lock<std::mutex> lock(shared_data->mutex);
        if (shared_data->indexes_of_timesteps_edited_by_ui.size() > 0)
        {



            // It is possible that the user will have edited multiple timesteps since the last loop.
            // This section finds the smallest index of the timesteps that have been edited.
            int smallest_index_edited_by_ui = simulation_data->size() - 1;
            for (int index : shared_data->indexes_of_timesteps_edited_by_ui)
            {

                if (index < smallest_index_edited_by_ui)
                {
                    smallest_index_edited_by_ui = index;
                }
            }

            shared_data->index_of_latest_timestep_simulated = smallest_index_edited_by_ui;
            shared_data->indexes_of_timesteps_edited_by_ui.clear();
        }



        if (shared_data->index_of_latest_timestep_simulated >= shared_data->index_of_latest_timestep_displayed + shared_data->buffer_size)
        {

            continue;
        }

        // ---------------------------------------Simulation Section---------------------------------
        int index_of_timestep_to_simulate = shared_data->index_of_latest_timestep_simulated + 1;

        int index_of_latest_timestep_simulated = shared_data->index_of_latest_timestep_simulated;

        std::unique_lock<std::mutex> lock2(simulation_data_mutex);
        simulation::Timestep input_timestep = simulation_data->at(index_of_latest_timestep_simulated);

        simulation::Timestep output_timestep = simulation::simulate_timestep(input_timestep, shared_data->atom_pair_library, shared_data);


        if (index_of_timestep_to_simulate == simulation_data->size())
        {
            simulation_data->push_back(output_timestep);
        }
        else
        {
            simulation_data->at(index_of_timestep_to_simulate) = output_timestep;
        }
        shared_data->index_of_latest_timestep_simulated++;

        lock.unlock();
        lock2.unlock();
    }
}

// Old versions
simulation::Timestep simulate_timestep(simulation::Config config, std::vector<atoms::Atom> atoms, double time, atoms::AtomPairLibrary &atom_pair_library)
{
    double total_potential_energy = 0;
    physics::evaluate_interactions(config, total_potential_energy, atoms, atom_pair_library); // Calculate forces

    for (atoms::Atom &atom : atoms)
    {
        physics::calculate_motion(config.timestep_size, atom);
    }

    double total_kinetic_energy = 0;
    for (atoms::Atom &atom : atoms)
    {
        total_kinetic_energy += atom.kinetic_energy;
    }

    time += config.timestep_size;

    return simulation::Timestep(config, atoms, total_kinetic_energy, total_potential_energy, time);
}

simulation::Timestep simulate_timestep(simulation::Timestep timestep, atoms::AtomPairLibrary &atom_pair_library)
{
    double total_potential_energy = 0;
    physics::evaluate_interactions(timestep.config, total_potential_energy, timestep.atoms, atom_pair_library); // Calculate forces

    for (atoms::Atom &atom : timestep.atoms)
    {
        physics::calculate_motion(timestep.config.timestep_size, atom);
    }

    double total_kinetic_energy = 0;
    for (atoms::Atom &atom : timestep.atoms)
    {
        total_kinetic_energy += atom.kinetic_energy;
    }

    timestep.time += timestep.config.timestep_size;

    return simulation::Timestep(timestep.config, timestep.atoms, total_kinetic_energy, total_potential_energy, timestep.time);
}

} // namespace simulation