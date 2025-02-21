#pragma once

#include <vector>
#include <stdexcept>
#include <map>
#include <string>
#include "Timestep.h"

class SimulationDataChannels
{
    public:
        SimulationDataChannels()
        {
            channel_count = 0;
            channel_limit_set = false;
            channel_limit = 0;
        }

        SimulationDataChannels(int channel_limit)
        {
            channel_count = 0;
            channel_limit_set = true;
            channel_limit = channel_limit;
        }


        std::map<std::string, std::vector<simulation::Timestep>> channels;
        int channel_count;
        bool channel_limit_set;
        int channel_limit;

        void add_channel(std::string channel_name, std::vector<simulation::Timestep> channel)
        {
            if (channel_limit_set && channel_count >= channel_limit)
            {
                throw std::runtime_error("Channel limit reached");
            }
            channels[channel_name] = channel;
            channel_count++;
        }

        std::vector<simulation::Timestep> get_channel(std::string channel_name)
        {
            return channels[channel_name];
        }

        void remove_channel(std::string channel_name)
        {
            channels.erase(channel_name);
            channel_count--;
        }

        void clear_channels()
        {
            channels.clear();
            channel_count = 0;
        }



};
