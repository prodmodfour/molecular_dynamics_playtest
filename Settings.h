#ifndef __Settings_h
#define __Settings_h
#include "Type_atom.h"
#include "file_functions.h"
#include "string_functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

class parameter
{
    private:
        std::string name;
        std::string value;
        std::string data_type;

        bool is_mutable;

    public:
        parameter(std::string name, int value, bool is_mutable = true)
        {
            this->value = std::to_string(value);
            this->data_type = "int";
            this->is_mutable = is_mutable;
        }

        parameter(std::string name, double value, bool is_mutable = true)
        {
            this->value = std::to_string(value);
            this->data_type = "double";
            this->is_mutable = is_mutable;
        }

        parameter(std::string name, std::string value, bool is_mutable = true)
        {
            this->value = value;
            this->data_type = "std::string";
            this->is_mutable = is_mutable;
        }

        void operator=(int value)
        {
            this->value = std::to_string(value);
            this->data_type = "int";
        }
        
        void operator=(double value)
        {
            this->value = std::to_string(value);
            this->data_type = "double";
        }
        
        void operator=(std::string value)
        {
            this->value = value;
            this->data_type = "std::string";
        }

        bool operator==(std::string name)
        {
            if (this->name == name)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        void print()
        {
            std::cout << data_type << " " << name << " " << value << std::endl;
        }

};

class Settings
{
    private:
        std::vector<parameter> parameters;
        int cubes_in_x;
        int cubes_in_y; 
        int cubes_in_z;
        int animation_step_duration;
        double simulation_timestep_size;
        double simulation_history_interval;
        int simulation_total_timesteps;

        double atom_spacing;
        Type_atom impact_atom_offsets;
        double energy_applied_to_impact_atom;

        double atom_mass;
        double atom_radius;

        double epsilon;
        double sigma;
        double r_cutoff;

        std::string mode; // This can be file or generate
        std::string atoms_filename; 

        double ev_to_j_per_mole;
        double j_per_mole_to_ev;
        double velocity_scale;
        double scaling;

        void _set_default_values()
        {
            parameter cubes_in_x("cubes_in_x", 10);
            parameter cubes_in_y("cubes_in_y", 10);
            parameter cubes_in_z("cubes_in_z", 10);
            
            parameter animation_step_duration("animation_step_duration", 5);
            parameter simulation_timestep_size("simulation_timestep_size", 0.001);
            parameter simulation_history_interval("simulation_history_interval");
            parameter simulation_total_timesteps("simulation_total_timesteps");

            parameter atom_spacing("atom_spacing");
            parameter impact_atom_offsets("impact_atom_offsets");
            parameter energy_applied_to_impact_atom("energy_applied_to_impact_atom");

            parameter atom_mass("atom_mass");
            parameter atom_radius("atom_radius");

            parameter epsilon("epsilon");
            parameter sigma("sigma");
            parameter r_cutoff("r_cutoff");

            parameter mode("mode");
            parameter atoms_filename("atoms_filename");

            parameter ev_to_j_per_mole("ev_to_j_per_mole");
            parameter j_per_mole_to_ev("j_per_mole_to_ev");
            parameter velocity_scale("velocity_scale");
            parameter scaling("scaling");

        }
        
    public:
        Settings(std::vector<std::string> arguments)
        {

        }

        // Printers
        void print()
        {

        }

        void print_help_message()
        {
            std::cout << "Console Flags" << std::endl;
            std::cout << "-help Show help message" << std::endl;

        }

};

#endif