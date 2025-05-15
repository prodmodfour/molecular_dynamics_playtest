#pragma once
#include <string>

namespace simulation {

typedef struct Config
{
    Config(double r_cutoff, double timestep_size);
    Config();

    double r_cutoff;
    double timestep_size;
} Config;

}