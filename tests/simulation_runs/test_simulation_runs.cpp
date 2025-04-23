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

#include <vector>
#include <iostream>
#include <QApplication>
#include <thread>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    //---------------------------------Simulation Section---------------------------------
    SharedData shared_data;
    Config config;
    std::vector<simulation::Timestep> simulation_data;
    std::thread simulation_thread(run_simulation, &shared_data, &simulation_data);
    // We detach because we control the simulation using shared_data
    simulation_thread.detach();

   //---------------------------------Visualisation Section--------------------------------

    ui::BasicDataLoader data_loader;
    ui::PlaybackSettings playback_settings(0);

    ui::MDVisualiser visualiser(nullptr, &data_loader, &playback_settings);
    visualiser.show();
    return app.exec();
}

