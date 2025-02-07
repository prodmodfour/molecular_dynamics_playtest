

#include "../../../simulation/Atom.h"
#include "../../../simulation/MDSimulator.h"
#include "../../../simulation/Config.h"
#include "../../../simulation/Timestep.h"
#include "../../../ui/MainWindow.h"
#include "../../../simulation/FCCGenerator.h"
#include "../../../simulation/Box.h"
#include "../../../simulation/UnitVectorCalculator.h"
#include "../../../simulation/KineticEnergyCalculator.h"
#include <vector>
#include <iostream>
#include <QApplication>


int main(int argc, char *argv[])
{   
    // Test 6: Create a FCC crystal lattice and fire an electron with 10 eV kinetic energy at it. See what happens.


    // Determine dimensions of the crystal
    int cubes_in_x = 8;
    int cubes_in_y = 8;
    int cubes_in_z = 4;
    double atom_spacing = 1.28;

    // Create a FCC crystal lattice
    std::string atom_type = "Cu";
    double mass = 63.546;
    double radius = 1.28;

    std::vector<Atom> atoms = generate_fcc(cubes_in_x, cubes_in_y, cubes_in_z, atom_spacing, atom_type, mass, radius);

    // Get dimensions of the box that contains the atoms
    Box box(atoms);

    // Create a moving atom that starts a set offset away from the center of the top place of the box
    Atom moving_atom(atom_type, mass, radius);
    double z_offset = 3;
    moving_atom.x = box.top_plane.center.x;
    moving_atom.y = box.top_plane.center.y;
    moving_atom.z = box.top_plane.center.z + z_offset;

    // Calculate the unit vector between the moving atom and the center of the top plane of the box
    Vector unit_vector = calculate_unit_vector_between_two_points(moving_atom.get_position(), box.top_plane.center);

    // Apply 10 eV kinetic energy to the moving atom
    double kinetic_energy = 10;
    apply_kinetic_energy(moving_atom, kinetic_energy, unit_vector);

    // Add the moving atom to the vector of atoms
    atoms.push_back(moving_atom);


    // Set up config for the simulation
    double r_cutoff = 4; 
    double timestep_size = 0.001;
    Config config(r_cutoff, timestep_size);


    // Create a MDSimulator
    MDSimulator simulator;


    // Run the simulation
    std::vector<Timestep> simulation_data;
    int total_timesteps = 100;
    for (int i = 0; i < total_timesteps; i++) 
    {
        Timestep timestep = simulator.simulate_timestep(config, atoms);
        simulation_data.push_back(timestep);
    }

    // Print the simulation data
    for (Timestep timestep : simulation_data)
    {
        std::cout << "Timestep: " << timestep.time << " " << "Atom position: " << timestep.atoms[0].x << ", " << timestep.atoms[0].y << ", " << timestep.atoms[0].z << std::endl;
    }

    // Launch the visualiser
    QApplication app(argc, argv);
    MainWindow main_window(simulation_data);
    main_window.show();
    app.exec();



    return 0;
}