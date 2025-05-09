// regression test for atom management features

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
    simulation::Config config;
    std::vector<simulation::Timestep> simulation_data;

    std::vector<atoms::Atom> atoms;
    simulation::Timestep first_timestep(config, atoms, 0, 0, 0);
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

