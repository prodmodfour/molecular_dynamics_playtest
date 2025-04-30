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
      visualiser(nullptr),
      simulation_data_set(false)
{
}

bool BasicDataLoader::load()
{
    if (!simulation_data_set)
    {
        return false;
    }
    
    std::vector<atoms::Atom> atoms = data->at(playback_settings->current_timestep_index).atoms;
    // Use the current timestep index to load the correct data element.
    visualiser->current_timestep_data = &data->at(playback_settings->current_timestep_index);
    return true;
}

void BasicDataLoader::setData(std::vector<simulation::Timestep>* data)
{
    this->data = data;
    simulation_data_set = true;
}

void BasicDataLoader::setPlaybackSettings(ui::PlaybackSettings* playback_settings)
{
    this->playback_settings = playback_settings;
}


void BasicDataLoader::setVisualiser(ui::MDVisualiser* visualiser)
{
    this->visualiser = visualiser;
}

void BasicDataLoader::updateLastTimestepIndex()
{
    if (simulation_data_set)
    {
        playback_settings->update_last_timestep_index(data->size() - 1);
    }
}

void BasicDataLoader::printDataSize()
{
    std::cout << "Data size: " << data->size() << std::endl;
}

} // namespace ui