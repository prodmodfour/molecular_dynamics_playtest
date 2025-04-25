// Test: Run a simple simulation 

#include "../../atoms/Atom.h"
#include "../../simulation/simulation_functions.h"
#include "../../simulation/Config.h"
#include "../../simulation/Timestep.h"
#include "../../ui/MDVisualiser.h"
#include "../../geometry/Box.h"
#include "../../geometry/geometry_functions.h"
#include "../../ui/data_loaders/BasicDataLoader.h"
#include "../../ui/PlaybackSettings.h"
#include "../../atoms/atom_generation_functions.h"
#include "../../atoms/Structure.h" // Included Structure
// Removed redundant includes for MDVisualiser, SharedData, simulation_data_mutex

#include <vector>
#include <iostream>
#include <QApplication>
#include <thread>
#include <string> // Include string

// Forward declaration if needed, or ensure SharedData.h is included if used directly here
class SharedData; 

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    //---------------------------------Simulation Section---------------------------------
    // Forward declare or include SharedData properly before use
    // If SharedData is complex, include its header: #include "../../ui/SharedData.h"
    SharedData shared_data; // Assuming SharedData is defined (needs header or definition)
    simulation::Config config;
    std::vector<simulation::Timestep> simulation_data;
    // std::vector<atoms::Atom> all_atoms; // This seems unused, Timestep manages atoms


    // Create the first timestep that kickstarts the simulation
    // Pass an empty vector for initial atoms, they will be added via add_structure/add_atom
    simulation::Timestep first_timestep(config, {}, 0, 0, 0);
    
    // Define basis atom and lattice constant for FCC structure
    atoms::Atom basis_atom("Cu", 63.546, 1.28);
    double lattice_constant = 3.61; // Angstroms for Copper
    std::string structure_name = "Copper_Block";
    std::string structure_type = "Fcc";

    // We start by creating an Fcc copper block using the correct constructor
    atoms::Structure copper_block(4, 4, 4, structure_name, structure_type, basis_atom, lattice_constant);
    first_timestep.add_structure(copper_block);

    double x_offset = 0.0;
    double y_offset = 0.0;
    double z_offset = 5.0;
    // Then, we add an impact atom
    atoms::Atom impact_atom("Cu", 63.546, 1.28); // Using the same type for simplicity
    
    // Get the box dimensions/center using the getter method
    geometry::Box block_box = first_timestep.get_structure(structure_name).get_box(); // Get structure then its box
    impact_atom.x = block_box.get_center().x + x_offset; // Example: Using box center
    impact_atom.y = block_box.get_center().y + y_offset;
    impact_atom.z = block_box.get_max_coords().z + z_offset; // Example: Placing above max Z

    first_timestep.add_atom(impact_atom);


    simulation_data.push_back(std::move(first_timestep)); // Move the timestep into the vector
    ui::PlaybackSettings playback_settings(0); // Initialize playback settings

    // Ensure SharedData.h is included for this thread function
    #include "../../ui/SharedData.h" // Need full definition for thread lambda
    std::thread simulation_thread(simulation::run_simulation, &shared_data, &simulation_data);
    // We detach because we control the simulation using shared_data
    simulation_thread.detach();

   //---------------------------------Visualisation Section--------------------------------
    ui::BasicDataLoader data_loader;
    data_loader.setData(&simulation_data);

    ui::MDVisualiser visualiser(nullptr, &data_loader, &playback_settings);
    visualiser.setSharedData(&shared_data); // Pass pointer to shared data
    visualiser.show();
    return app.exec();
}
