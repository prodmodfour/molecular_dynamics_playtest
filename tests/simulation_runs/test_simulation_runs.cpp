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

#include <vector>
#include <iostream>
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set up simulation data

    // Create a basic data loader
    ui::BasicDataLoader data_loader;

    // Create a playback settings
    ui::PlaybackSettings playback_settings(0);

    // Create a visualiser
    ui::MDVisualiser visualiser(nullptr, &data_loader, &playback_settings);
    visualiser.show();
    return app.exec();
}

