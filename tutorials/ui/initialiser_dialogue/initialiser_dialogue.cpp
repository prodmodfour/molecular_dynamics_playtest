#include "../../atoms/Atom.h"
#include "../../simulation/simulation_functions.h"
#include "../../simulation/InitialParameters.h"
#include "../../simulation/Config.h"
#include "../../simulation/Timestep.h"
#include "../../ui/MDVisualiser.h"
#include "../../geometry/Box.h"
#include "../../geometry/geometry_functions.h"
#include "../../ui/data_loaders/BasicDataLoader.h"
#include "../../ui/PlaybackSettings.h"
#include "../../ui/InitialiserDialog.h"
#include "../../atoms/atom_generation_functions.h"

#include <vector>
#include <iostream>
#include <QApplication>


int main(int argc, char *argv[])
{   
    // Create a simulation using the initialiser dialogue
    QApplication app(argc, argv);
    ui::InitialiserDialog initialiser_dialog;
    simulation::InitialParameters initial_parameters;
    if (initialiser_dialog.exec() == QDialog::Accepted)
    {
        initial_parameters = initialiser_dialog.getInitialParameters();
    }



    // Create a FCC crystal lattice
    std::string atom_type = "Cu";
    double mass = 63.546;
    double radius = 1.28;

    std::vector<atoms::Atom> atoms = atoms::generate_atoms_in_fcc_pattern(
    initial_parameters.cubes_in_x, initial_parameters.cubes_in_y, initial_parameters.cubes_in_z, 
    initial_parameters.atom_spacing, 
    atom_type, mass, radius);

    // Get dimensions of the box that contains the atoms
    geometry::Box box(atoms);

    // Create a moving atom that starts a set offset away from the center of the top place of the box
    atoms::Atom moving_atom(atom_type, mass, radius);
    moving_atom.x = box.top_plane.center.x + initial_parameters.impact_atom_offset.x;
    moving_atom.y = box.top_plane.center.y + initial_parameters.impact_atom_offset.y;
    moving_atom.z = box.top_plane.center.z + initial_parameters.impact_atom_offset.z;

    // Calculate the unit vector between the moving atom and the center of the top plane of the box
    geometry::Vector unit_vector = geometry::calculate_unit_vector_between_two_points(moving_atom.get_position(), box.top_plane.center);

    // Apply kinetic energy to the moving atom
    double kinetic_energy = initial_parameters.impact_energy;
    physics::apply_kinetic_energy(moving_atom, kinetic_energy, unit_vector);

    // Add the moving atom to the vector of atoms
    atoms.push_back(moving_atom);

    // Set up Atom Pair Library
    atoms::AtomPairLibrary atom_pair_library;

    // Run the simulation
    std::vector<simulation::Timestep> simulation_data;
    int total_timesteps = 1000;
    double time = 0;
    for (int i = 0; i < total_timesteps; i++) 
    {
        simulation::Timestep timestep = simulation::simulate_timestep(initial_parameters.config, atoms, time, atom_pair_library);
        simulation_data.push_back(timestep);
    }

    int last_timestep_index = simulation_data.size() - 1;
    // Set up playback settings
    ui::PlaybackSettings playback_settings(last_timestep_index);

    // Create a data loader
    ui::BasicDataLoader data_loader;
    data_loader.setData(&simulation_data);
    data_loader.setPlaybackSettings(&playback_settings);


    // Launch the visualiser
    ui::MDVisualiser visualiser(nullptr, &data_loader, &playback_settings);
    visualiser.show();
    std::cout << "Visualiser shown" << std::endl;
    


    return 0;
}