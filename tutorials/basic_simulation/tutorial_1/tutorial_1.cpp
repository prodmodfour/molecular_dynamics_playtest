#include "../../../atoms/Atom.h"
#include "../../../atoms/AtomPairLibrary.h"
#include "../../../simulation/simulation_functions.h"
#include "../../../simulation/Config.h"
#include "../../../simulation/Timestep.h"
#include "../../../ui/MDVisualiser.h"
#include <vector>
#include <iostream>
#include <QApplication>





int main(int argc, char *argv[])
{   
    // Test 1: Create a singe atom with no initial velocity. Watch it not move for 100 timesteps.



    // Create a single atom
    std::string atom_type = "Cu";
    double mass = 63.546;
    double radius = 1.28;
    atoms::Atom atom(atom_type, mass, radius);
    std::vector<atoms::Atom> atoms;
    atoms.push_back(atom);


    // Set up config for the simulation
    double r_cutoff = 0; // It's just one atom, so no cutoff is needed
    double timestep_size = 0.001;
    simulation::Config config(r_cutoff, timestep_size);


    // Set up Atom Pair Library
    atoms::AtomPairLibrary atom_pair_library;

    // Run the simulation
    std::vector<simulation::Timestep> simulation_data;
    int total_timesteps = 100;
    double time = 0;
    for (int i = 0; i < total_timesteps; i++) 
    {
        simulation::Timestep timestep = simulation::simulate_timestep(config, atoms, time, atom_pair_library);
        simulation_data.push_back(timestep);
    }

    // Print the simulation data
    for (simulation::Timestep timestep : simulation_data)
    {
        std::cout << "Timestep: " << timestep.time << " " << "Atom position: " << timestep.atoms[0].x << ", " << timestep.atoms[0].y << ", " << timestep.atoms[0].z << std::endl;
    }

    // Launch the visualiser
    QApplication app(argc, argv);
    ui::MDVisualiser visualiser(simulation_data);
    visualiser.show();
    app.exec();



    return 0;
}


