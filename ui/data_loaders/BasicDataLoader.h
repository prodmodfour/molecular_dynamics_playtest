#pragma once
#include <vector>
#include <stdexcept>  

#include "DataLoader.h"

// Needed to forward declare this to get it to compile
namespace simulation {
    class Timestep;
}

class SharedData;

namespace ui {
   
class PlaybackSettings;
class MDVisualiser;

class BasicDataLoader : public ui::DataLoader
{
public:
    BasicDataLoader();

    // Overridden functions
    bool load() override;

    // Setter methods
    void setData(std::vector<simulation::Timestep>* data);
    void setPlaybackSettings(ui::PlaybackSettings* playback_settings);
    void setVisualiser(ui::MDVisualiser* visualiser);
    void setSharedData(SharedData* shared_data);

    void updateLastTimestepIndex();
    void printDataSize();
    bool simulation_data_set;

    void clearData();

private:
    std::vector<simulation::Timestep>* data;
    ui::PlaybackSettings* playback_settings;
    ui::MDVisualiser* visualiser;
    SharedData* shared_data;
};

} // namespace ui


