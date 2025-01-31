#pragma once
#include <string>

class Config
{
    public:
        Config(double r_cutoff, double timestep_size)
        {
            this->r_cutoff = r_cutoff;
            this->timestep_size = timestep_size;
        }

        double r_cutoff;
        double timestep_size;
};