#pragma once
#include <vector>
#include <stdexcept>  

#include "DataLoader.h"

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


    bool load() override;


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


