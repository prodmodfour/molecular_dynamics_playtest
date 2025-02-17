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
        }

        int direction;
        int speed; 
        bool pause;

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
};

}