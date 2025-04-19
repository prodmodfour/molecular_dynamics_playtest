#pragma once

#include "../simulation/Timestep.h"
#include <vector>

namespace ui {

class PlaybackSettings
{
    public:
        PlaybackSettings(int last_timestep_index)
        {
            this->direction = 1;
            this->speed = 1;
            this->pause = true;
            this->current_timestep_index = 0;
            this->last_timestep_index = last_timestep_index;
        }

        int direction;
        int speed; 
        bool pause;
        int current_timestep_index;
        int last_timestep_index;

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

            if (current_timestep_index < 0)
            {
                current_timestep_index = 0;
                return;
            }
            if (current_timestep_index > last_timestep_index)
            {
                current_timestep_index = last_timestep_index;
                return;
            }
            if (current_timestep_index == last_timestep_index && direction == 1)
            {
                return;
            }
            if (current_timestep_index == 0 && direction == -1)
            {
                return;
            }
            if (current_timestep_index == 0 && last_timestep_index == 0)
            {
                return;
            }

            current_timestep_index += direction * speed;
        }

        void update_last_timestep_index(int new_last_timestep_index)
        {
            last_timestep_index = new_last_timestep_index;
        }


    
};

}