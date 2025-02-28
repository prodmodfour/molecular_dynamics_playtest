#pragma once

#include "Config.h"
#include "../geometry/Vector.h"

namespace simulation {

class InitialParameters
{
    public:
        InitialParameters(double impact_energy, double r_cutoff, double timestep_size, 
        int cubes_in_x, int cubes_in_y, int cubes_in_z, 
        double impact_atom_x_offset, double impact_atom_y_offset, double impact_atom_z_offset,
        int number_of_timesteps, double atom_spacing)
        {
            this->impact_energy = impact_energy;
            this->config = Config(r_cutoff, timestep_size);
            this->cubes_in_x = cubes_in_x;
            this->cubes_in_y = cubes_in_y;
            this->cubes_in_z = cubes_in_z;
            this->impact_atom_offset = geometry::Vector(impact_atom_x_offset, impact_atom_y_offset, impact_atom_z_offset);
            this->number_of_timesteps = number_of_timesteps;
            this->atom_spacing = atom_spacing;
        }

        InitialParameters()
        {
            
        }

        double impact_energy;
        geometry::Vector impact_atom_offset;
        simulation::Config config;
        int cubes_in_x;
        int cubes_in_y;
        int cubes_in_z;
        int number_of_timesteps;
        double atom_spacing;

};

}