#pragma once

#include "../simulation/Timestep.h"
#include <vector>
#include <algorithm>   
#include <cassert>     

namespace ui {

class PlaybackSettings
{
    public:
        PlaybackSettings(int last_timestep_index)
        {
            this->direction = 1;
            this->speed = 1;
            this->pause = false;
            this->current_timestep_index = 0;
            this->last_timestep_index = last_timestep_index;
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

        void next_timestep()
        {
            step_timestep(current_timestep_index, last_timestep_index, direction, speed);
        }

        void step_timestep(int& index,
                        int  last,
                        int  direction,
                        int  speed = 1)
        {
            assert(direction == -1 || direction == 1);
            assert(speed > 0);
            assert(last >= 0);

            index = std::clamp(index, 0, last);

            if (last == 0 ||
                (index == last && direction > 0) ||
                (index == 0    && direction < 0))
            {
                return;
            }

            index = std::clamp(index + direction * speed, 0, last);
        }


        void update_last_timestep_index(int new_last_timestep_index)
        {
            last_timestep_index = new_last_timestep_index;
        }

        void reset()
        {
            current_timestep_index = 0;
            direction = 1;
            speed = 1;
            pause = true;
        }

        bool is_paused() { return pause; }

        int get_current_timestep_index() { return current_timestep_index; }

        int get_current_speed() { return speed; }

    private:
        int direction;
        int speed; 
        bool pause;
        int current_timestep_index;
        int last_timestep_index;
    
};

}