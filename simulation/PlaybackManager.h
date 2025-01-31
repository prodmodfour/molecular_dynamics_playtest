#pragma once

#include "Timestep.h"
#include <vector>

class PlaybackManager
{
    public:
        PlaybackManager(int total_timesteps)
        {
            this->current_timestep = 0;
            this->total_timesteps = total_timesteps;
            this->direction = 1;
            this->speed = 1;
            this->pause = false;
        }

        int current_timestep;
        int total_timesteps;
        int direction;
        int speed; 
        bool pause;

        void update_timestep()
        {
            if (pause)
            {
                return;
            }
            current_timestep += direction * speed;
            if (current_timestep >= total_timesteps)
            {
                current_timestep = total_timesteps - 1;
            }
            else if (current_timestep < 0)
            {
                current_timestep = 0;
            }
        }

        void change_direction()
        {
            direction = -direction;
        }

        void change_speed(int new_speed)
        {
            speed = new_speed;
        }

        void toggle_pause()
        {
            pause = !pause;
        }
}