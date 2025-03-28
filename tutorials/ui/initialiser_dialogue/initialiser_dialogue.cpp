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
#include "../../ui/MDVisualiser.h"

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

    initial_parameters.print();

    // Create a FCC crystal lattice
    std::string atom_type = "Cu";
    double mass = 63.546;
    double radius = 1.28;

    std::vector<atoms::Atom> atoms = atoms::generate_atoms_in_fcc_pattern(
    initial_parameters.cubes_in_x, initial_parameters.cubes_in_y, initial_parameters.cubes_in_z, 
    initial_parameters.atom_spacing, 
    atom_type, mass, radius);

    std::cout << "Number of atoms: " << atoms.size() << std::endl;
    // Print all atoms
    for (int i = 0; i < atoms.size(); i++)
    {
        // Print the atom position
        std::cout << "Atom " << i << ": " << atoms[i].x << ", " << atoms[i].y << ", " << atoms[i].z << std::endl;
        // Print the atom velocity
        std::cout << "Atom " << i << " velocity: " << atoms[i].vx << ", " << atoms[i].vy << ", " << atoms[i].vz << std::endl;
        // Print the atom force
        std::cout << "Atom " << i << " force: " << atoms[i].fx << ", " << atoms[i].fy << ", " << atoms[i].fz << std::endl;
    }

    // Get dimensions of the box that contains the atoms
    geometry::Box box(atoms);

    // Create a moving atom that starts a set offset away from the center of the top plane of the box
    atoms::Atom moving_atom(atom_type, mass, radius);
    moving_atom.x = box.top_plane.center.x + initial_parameters.impact_atom_offset.x;
    moving_atom.y = box.top_plane.center.y + initial_parameters.impact_atom_offset.y;
    moving_atom.z = box.top_plane.center.z + initial_parameters.impact_atom_offset.z;

    std::cout << "Moving atom: " << moving_atom.x << ", " << moving_atom.y << ", " << moving_atom.z << std::endl;

    // Calculate the unit vector between the moving atom and the center of the top plane of the box
    std::cout << "Box top plane center: " << box.top_plane.center.x << ", " << box.top_plane.center.y << ", " << box.top_plane.center.z << std::endl;
    std::cout << "Moving atom position: " << moving_atom.get_position().x << ", " << moving_atom.get_position().y << ", " << moving_atom.get_position().z << std::endl;
    geometry::Vector unit_vector = geometry::calculate_unit_vector_from_point1_to_point2(moving_atom.get_position(), box.top_plane.center);

    std::cout << "Unit vector: " << unit_vector.x << ", " << unit_vector.y << ", " << unit_vector.z << std::endl;
    // Apply kinetic energy to the moving atom
    double kinetic_energy = initial_parameters.impact_energy;
    physics::apply_kinetic_energy(moving_atom, kinetic_energy, unit_vector);
    std::cout << "kinetic energy: " << kinetic_energy << std::endl;


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

  

    std::cout << "Simulation data size: " << simulation_data.size() << std::endl;
    // Print the last timestep
    std::cout << "Last timestep: " << simulation_data[simulation_data.size() - 1].time << std::endl;

    // Print the last system state
    std::cout << "Last system state: " << std::endl;
    for (int i = 0; i < atoms.size(); i++)
    {
        // Print the atom position
        std::cout << "Atom " << i << ": " << atoms[i].x << ", " << atoms[i].y << ", " << atoms[i].z << std::endl;
        // Print the atom velocity
        std::cout << "Atom " << i << " velocity: " << atoms[i].vx << ", " << atoms[i].vy << ", " << atoms[i].vz << std::endl;
        // Print the atom force
        std::cout << "Atom " << i << " force: " << atoms[i].fx << ", " << atoms[i].fy << ", " << atoms[i].fz << std::endl;
    }
  // show position, velocity and forces on last atom at the 2nd timestep
    std::cout << "Position of last atom at 2nd timestep: " << simulation_data[1].atoms[simulation_data[1].atoms.size() - 1].x << ", " << simulation_data[1].atoms[simulation_data[1].atoms.size() - 1].y << ", " << simulation_data[1].atoms[simulation_data[1].atoms.size() - 1].z << std::endl;
    std::cout << "Velocity of last atom at 2nd timestep: " << simulation_data[1].atoms[simulation_data[1].atoms.size() - 1].vx << ", " << simulation_data[1].atoms[simulation_data[1].atoms.size() - 1].vy << ", " << simulation_data[1].atoms[simulation_data[1].atoms.size() - 1].vz << std::endl;
    std::cout << "Force of last atom at 2nd timestep: " << simulation_data[1].atoms[simulation_data[1].atoms.size() - 1].fx << ", " << simulation_data[1].atoms[simulation_data[1].atoms.size() - 1].fy << ", " << simulation_data[1].atoms[simulation_data[1].atoms.size() - 1].fz << std::endl;


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
    


    return app.exec();
}