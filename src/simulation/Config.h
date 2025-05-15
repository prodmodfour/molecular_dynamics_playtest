#pragma once
#include <string>

namespace simulation {

class Config
{
    public:
        Config(double r_cutoff, double timestep_size)
        {
            this->r_cutoff = r_cutoff;
            this->timestep_size = timestep_size;
        }

        Config()
        {
            this->r_cutoff = 6;
            this->timestep_size = 0.001;
        }

        double r_cutoff;
        double timestep_size;
};

}