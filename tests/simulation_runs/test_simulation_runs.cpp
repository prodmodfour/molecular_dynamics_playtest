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
#include "../../ui/MDVisualiser.h"
#include "../../ui/SharedData.h"
#include "../../simulation/simulation_data_mutex.h"
#include "../../atoms/Structure.h"


#include <vector>
#include <iostream>
#include <QApplication>
#include <thread>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    //---------------------------------Simulation Section---------------------------------
    SharedData shared_data;
    simulation::Config config;
    std::vector<simulation::Timestep> simulation_data;
    std::vector<atoms::Atom> all_atoms;


    // Create the first timestep that kickstarts the simulation
    simulation::Timestep first_timestep(config, all_atoms, 0, 0, 0);
    // We start by creating an Fcc copper block
    atoms::Structure copper_block(4, 4, 4, "Copper_Block`");
    first_timestep.add_structure(copper_block);

    double x_offset = 0.0;
    double y_offset = 0.0;
    double z_offset = 5.0;
    // Then, we add an impact atom
    atoms::Atom impact_atom("Cu", 63.546, 1.28);
    impact_atom.x = copper_block.box.top_plane.center.x + x_offset;
    impact_atom.y = copper_block.box.top_plane.center.y + y_offset;
    impact_atom.z = copper_block.box.top_plane.center.z + z_offset;
    first_timestep.add_atom(impact_atom);


    simulation_data.push_back(first_timestep);
    ui::PlaybackSettings playback_settings(0);

    std::thread simulation_thread(simulation::run_simulation, &shared_data, &simulation_data);
    // We detach because we control the simulation using shared_data
    simulation_thread.detach();

   //---------------------------------Visualisation Section--------------------------------
    ui::BasicDataLoader data_loader;
    data_loader.setData(&simulation_data);


    ui::MDVisualiser visualiser(nullptr, &data_loader, &playback_settings);
    visualiser.setSharedData(&shared_data);
    visualiser.show();
    return app.exec();
}

