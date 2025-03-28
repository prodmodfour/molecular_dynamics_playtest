#pragma once
#include <vector>
#include <stdexcept>  

#include "DataLoader.h"

namespace simulation {
    class Timestep;
}

namespace ui {
   
class PlaybackSettings;
class MDVisualiser;

class BasicDataLoader : public ui::DataLoader
{
public:
    BasicDataLoader();

    // Overridden functions
    void load() override;

    // Setter methods
    void setData(std::vector<simulation::Timestep>* data);
    void setPlaybackSettings(ui::PlaybackSettings* playback_settings);
    void setDataOutputPointer(simulation::Timestep* data_output_pointer);
    void setVisualiser(ui::MDVisualiser* visualiser);

private:
    std::vector<simulation::Timestep>* data;
    ui::PlaybackSettings* playback_settings;
    simulation::Timestep* data_output_pointer;
    ui::MDVisualiser* visualiser;
};

} // namespace ui


