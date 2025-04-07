// Test: Launch the UI without crashing

#include "../../ui/MDVisualiser.h"
#include "../../ui/data_loaders/BasicDataLoader.h"
#include "../../ui/PlaybackSettings.h"


int main()
{
    // Create a basic data loader
    ui::BasicDataLoader data_loader;

    // Create a playback settings
    ui::PlaybackSettings playback_settings;



    // Create a visualiser
    ui::MDVisualiser visualiser(&data_loader, &playback_settings);
    visualiser.show();
    return 0;
}

