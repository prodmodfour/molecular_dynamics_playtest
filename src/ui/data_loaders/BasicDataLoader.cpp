#include "BasicDataLoader.h"
#include "DataLoader.h" 
#include "../../simulation/Timestep.h"
#include "../PlaybackSettings.h"
#include "../main_visualiser_widgets/MDVisualiser.h"
#include "../../atoms/Atom.h"
#include <iostream>
#include <vector>
#include <mutex>
#include "../../simulation/simulation_data_mutex.h"

namespace ui
{

BasicDataLoader::BasicDataLoader()
    : data(nullptr),
      playback_settings(nullptr),
      visualiser(nullptr),
      simulation_data_set(false),
      shared_data(nullptr)
{
}

bool BasicDataLoader::load()
{
    if (!simulation_data_set)
    {
        return false;
    }
    
    std::vector<atoms::Atom> atoms = data->at(playback_settings->get_current_timestep_index()).atoms;
    visualiser->setCurrentTimestepData(&data->at(playback_settings->get_current_timestep_index()));
    return true;
}

void BasicDataLoader::setData(std::vector<simulation::Timestep>* data)
{
    this->data = data;
    simulation_data_set = true;
}

void BasicDataLoader::setSharedData(SharedData* shared_data)
{
    this->shared_data = shared_data;
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

void BasicDataLoader::clearData()
{
    std::unique_lock<std::mutex> lock(simulation_data_mutex);
    simulation::Timestep first_timestep = data->at(0);
    first_timestep.atoms.clear();
    data->at(0) = first_timestep;

    std::unique_lock<std::mutex> lock2(shared_data->mutex);
    shared_data->indexes_of_timesteps_edited_by_ui.push_back(0);
}

} // namespace ui