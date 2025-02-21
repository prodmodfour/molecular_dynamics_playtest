#pragma once
#include <vector>

#include "DataLoader.h"
#include "../../simulation/SimulationDataChannels.h"
#include "../../simulation/Timestep.h"
#include "../PlaybackSettings.h"


class BasicDataLoader : public DataLoader
{
    public:
        std::vector<simulation::Timestep> data;
        PlaybackSettings playback_settings;
        BasicDataLoader(std::vector<simulation::Timestep> &data, PlaybackSettings &playback_settings)
        {
            this->data = data;
            this->playback_settings = playback_settings;
        }
        simulation::Timestep load() 
        {
            return data[playback_settings.current_timestep];
        }

};
