#pragma once
#include <vector>

#include "DataLoader.h"
#include "../../simulation/SimulationDataChannels.h"
#include "../../simulation/Timestep.h"
#include "../PlaybackSettings.h"

namespace ui
{
class BasicDataLoader : public ui::DataLoader
{
    public:
        std::vector<simulation::Timestep>* data;
        ui::PlaybackSettings* playback_settings;
        simulation::Timestep* data_output_pointer;

        BasicDataLoader()
        {
            data_output_pointer = nullptr;
            playback_settings = nullptr;
            data = nullptr;
        }

        void load() override
        {
            if (data_output_pointer == nullptr)
            {
                throw std::invalid_argument("Data output pointer is not set");
            }

            *data_output_pointer = data->at(playback_settings->current_timestep_index);
        }

        void setData(std::vector<simulation::Timestep>* data)
        {
            this->data = data;
        }

        void setPlaybackSettings(ui::PlaybackSettings* playback_settings)
        {
            this->playback_settings = playback_settings;
        }

        void setDataOutputPointer(simulation::Timestep* data_output_pointer)
        {
            this->data_output_pointer = data_output_pointer;
        }

        

};
} // namespace ui
