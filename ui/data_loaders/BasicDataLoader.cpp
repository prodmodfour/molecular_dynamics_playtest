#include "BasicDataLoader.h"
#include "DataLoader.h" 
#include "../../simulation/Timestep.h"
#include "../PlaybackSettings.h"
#include "../MDVisualiser.h"
#include "../../atoms/Atom.h"
#include <iostream>
#include <vector>

namespace ui
{

BasicDataLoader::BasicDataLoader()
    : data(nullptr),
      playback_settings(nullptr),
      data_output_pointer(nullptr),
      visualiser(nullptr)
{
}

void BasicDataLoader::load()
{


    std::vector<atoms::Atom> atoms = data->at(playback_settings->current_timestep_index).atoms;
    // Use the current timestep index to load the correct data element.
    visualiser->current_timestep_data = &data->at(playback_settings->current_timestep_index);
}

void BasicDataLoader::setData(std::vector<simulation::Timestep>* data)
{
    this->data = data;
}

void BasicDataLoader::setPlaybackSettings(ui::PlaybackSettings* playback_settings)
{
    this->playback_settings = playback_settings;
}


void BasicDataLoader::setVisualiser(ui::MDVisualiser* visualiser)
{
    this->visualiser = visualiser;
}

} // namespace ui