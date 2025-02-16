

#include "../../../physics/Atom.h"
#include "../../../simulation/MDSimulator.h"
#include "../../../simulation/Config.h"
#include "../../../simulation/Timestep.h"
#include "../../../ui/MainWindow.h"
#include "../../../geometry/FCCGenerator.h"
#include <vector>
#include <iostream>
#include <QApplication>




int main(int argc, char *argv[])
{   
    // Test 4: Create a FCC crystal lattice and watch it be stable. No intiial velocities.


    // Determine dimensions of the crystal
    int cubes_in_x = 1;
    int cubes_in_y = 1;
    int cubes_in_z = 1;
    double atom_spacing = 1.28;

    // Create a FCC crystal lattice
    std::string atom_type = "Cu";
    double mass = 63.546;
    double radius = 1.28;

    std::vector<atoms::Atom> atoms = geometry::generate_atoms_in_fcc_pattern(
    cubes_in_x, cubes_in_y, cubes_in_z, 
    atom_spacing, atom_type,
     mass, radius);


    // Set up config for the simulation
    double r_cutoff = 4; 
    double timestep_size = 0.001;
    simulation::Config config(r_cutoff, timestep_size);




    // Run the simulation
    std::vector<simulation::Timestep> simulation_data;
    int total_timesteps = 100;
    double time = 0;
    for (int i = 0; i < total_timesteps; i++) 
    {
        simulation::Timestep timestep = simulation::simulate_timestep(config, atoms, time);
        simulation_data.push_back(timestep);
    }

    // Print the simulation data
    for (simulation::Timestep timestep : simulation_data)
    {
        std::cout << "Timestep: " << timestep.time << " " << "Atom position: " << timestep.atoms[0].x << ", " << timestep.atoms[0].y << ", " << timestep.atoms[0].z << std::endl;
    }

    // Launch the visualiser
    QApplication app(argc, argv);
    ui::MainWindow main_window(simulation_data);
    main_window.show();
    app.exec();



    return 0;
}