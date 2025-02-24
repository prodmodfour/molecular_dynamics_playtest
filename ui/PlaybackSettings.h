#pragma once

#include "../simulation/Timestep.h"
#include <vector>

namespace ui {

class PlaybackSettings
{
    public:
        PlaybackSettings()
        {
            this->direction = 1;
            this->speed = 1;
            this->pause = false;
            this->current_timestep_index = 0;
        }

        int direction;
        int speed; 
        bool pause;
        int current_timestep_index;

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

        void next_timestep()
        {
            current_timestep_index += direction * speed;
        }


    
};

}