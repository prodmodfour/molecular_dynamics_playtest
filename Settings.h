#ifndef __Settings_h
#define __Settings_h
#include "Atom.h"
#include "file_functions.h"
#include "string_functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <stdexcept>

// Converts any value to a string representation
template<typename T>
std::string to_string(T value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// Converts a string back to the given type T
template<typename T>
T from_string(const std::string& str)
{
    std::istringstream iss(str);
    T value;
    iss >> value;
    return value;
}

class parameter
{
    private:
        std::string name;
        std::vector<std::string> values;  
        std::string data_type;
        bool is_mutable;

    public:
        // Default constructor initializing empty values
        parameter() : name(""), is_mutable(true), data_type("double") {}


        // Constructor for multiple double values
        template<typename... Args>
        parameter(std::string name, double first, Args... args)
            : name(name), is_mutable(true)
        {
            this->data_type = "double";  
            add_values(first, args...);
        }

        // Constructor for multiple int values
        template<typename... Args>
        parameter(std::string name, int first, Args... args)
            : name(name), is_mutable(true)
        {
            this->data_type = "int";  
            add_values(first, args...);
        }

        // Constructor for multiple string values
        template<typename... Args>
        parameter(std::string name, const std::string& first, Args... args)
            : name(name), is_mutable(true)
        {
            this->data_type = "std::string";  
            add_values(first, args...);
        }

         // Constructor for boolean values
        parameter(std::string name, bool value)
            : name(name), is_mutable(true)
        {
            this->data_type = "bool";
            values.push_back(value ? "true" : "false");
        }

        // Expands variadic arguments and stores them as strings
        template<typename T, typename... Args>
        void add_values(T first, Args... rest)
        {
            values.push_back(to_string(first));
            add_values(rest...);
        }

        void add_values() {}

        // Overload conversion to double array
        operator double*() const
        {
            if (data_type != "double")
            {
                throw std::runtime_error("Invalid data type conversion to double.");
            }

            static double arr[3]; // Assuming we need 3 values
            for (size_t i = 0; i < 3 && i < values.size(); ++i)
            {
                arr[i] = from_string<double>(values[i]);
            }
            return arr;
        }



        std::string get_data_type() const
        {
            return data_type;
        }


        std::vector<std::string> get_values() const
        {
            return values;
        }
};

class Settings
{
    private:
        std::map<std::string, parameter> parameters;
        std::string default_filename = "settings.txt"; 


        // Internal helper to add a new parameter to the map
        void add_parameter(const std::string& name, const parameter& parameter)
        {
            parameters[name] = parameter;
        }


        // Creates a default settings file if one doesn't exist
        void create_default_settings_file()
        {
            std::ofstream file(default_filename);
            if (!file)
            {
                throw std::runtime_error("Unable to create default settings.txt");
            }

            // Add default parameters with their data types
            file << "atom_radius double 1.28\n";
            file << "atom_mass double 63.546\n";
            file << "animation_step_duration int 1\n";
            file << "cubes_in_x int 5\n";
            file << "cubes_in_y int 5\n";
            file << "cubes_in_z int 5\n";
            file << "atom_spacing double 3.61\n";
            file << "energy_applied_to_impact_atom double 10\n";
            file << "impact_atom_x_offset double 0\n";
            file << "impact_atom_y_offset double 3\n";
            file << "impact_atom_z_offset double 0\n";
            file << "ev_to_j_per_mole double 96400.0\n";
            file << "j_per_mole_to_ev double 1.037e-5\n";
            file << "scaling double 0.01\n";
            file << "epsilon double 0\n";
            file << "sigma double 2.285\n";
            file << "rcutoff double 0\n";
            file << "velocity_scale double 0\n";
            file << "simulation_history_interval int 50\n";
            file << "simulation_timestep_size double 0.001\n";
            file << "simulation_total_timesteps int 1000\n";
            file << "atom_mode std::string generate\n"; // from_file or generate
            file << "atom_filename std::string config.txt\n"; // 
            file << "add_impact_on bool true\n";
            file << "parallel_projection_on bool false\n";
            file << "animation_on bool true\n";
            file << "simulation_on bool true\n";



            file.close();
        }

        void calculate_derived_variables()
        {
            double ev_j_per_mole = get_ev_to_j_per_mole();
            double epsilon = 0.4802 * ev_j_per_mole;
            set_epsilon(epsilon);

            double sigma = get_sigma();
            double r_cutoff = 2.5 * sigma;
            set_rcutoff(r_cutoff);

            double scaling = get_scaling();
            double simulation_timestep_size = get_simulation_timestep_size();
            double atom_mass = get_atom_mass();
            double velocity_scale =  scaling * simulation_timestep_size / atom_mass;
            set_velocity_scale(velocity_scale);


        }

    public:

        Settings(){}

        Settings(std::vector<std::string> arguments)
        {

            load_from_file();
            update_from_flags(arguments);
            save_to_file();
            calculate_derived_variables();

        }

        void update_from_flags(std::vector<std::string> arguments)
        {
            for (int i = 0; i < arguments.size(); i++)
            {
                std::string name = arguments[i];
                if (name == "-cubes")
                {
                    set_cubes_in_x(std::stoi(arguments[i + 1]));
                    set_cubes_in_y(std::stoi(arguments[i + 2]));
                    set_cubes_in_z(std::stoi(arguments[i + 3]));
                    continue;

                }


                auto it = parameters.find(name);
                if (it != parameters.end())
                {
                    const parameter& param = it->second;

                    if(param.get_data_type() == "double")
                    {
                        double value = std::stod(arguments[i + 1]);
                        add_parameter(name, parameter(name, value));
                    }
                    else if(param.get_data_type() == "int")
                    {
                        int value = std::stoi(arguments[i + 1]);
                        add_parameter(name, parameter(name, value));
                    }
                    else if(param.get_data_type() == "std::string")
                    {
                        std::string value = arguments[i + 1];
                        add_parameter(name, parameter(name, value));
                    }
                    else if(param.get_data_type() == "bool")
                    {
                        std::string value = arguments[i + 1];
                        if (arguments[i + 1] == "true")
                        {
                            add_parameter(name, parameter(name, true));              
                        }
                        else if (arguments[i + 1] == "false")
                        {
                            add_parameter(name, parameter(name, false));     
                        }
                        
                    }
                }


            }
        }

       // Retrieve the parameter's value(s) based on its data type
        double* get_double(const std::string& name) const
        {
            auto it = parameters.find(name);
            if (it != parameters.end())
            {
                const parameter& param = it->second;

                if (param.get_data_type() == "double")
                {
                    static double values[3];
                    for (size_t i = 0; i < param.get_values().size(); ++i)
                    {
                        values[i] = from_string<double>(param.get_values()[i]);
                    }
                    return values;
                }
                else
                {
                    throw std::runtime_error("Requested data type does not match parameter data type.");
                }
            }
            std::cout << name << std::endl;
            throw std::runtime_error("Parameter not found");
        }

        int* get_int(const std::string& name) const
        {
            auto it = parameters.find(name);
            if (it != parameters.end())
            {
                const parameter& param = it->second;

                if (param.get_data_type() == "int")
                {
                    static int values[3];
                    for (size_t i = 0; i < param.get_values().size(); ++i)
                    {
                        values[i] = from_string<int>(param.get_values()[i]);
                    }
                    return values;
                }
                else
                {
                    throw std::runtime_error("Requested data type does not match parameter data type.");
                }
            }
            std::cout << name << std::endl;
            throw std::runtime_error("Parameter not found");
        }

        std::string get_string(const std::string& name) const
        {
            auto it = parameters.find(name);
            if (it != parameters.end())
            {
                const parameter& param = it->second;

                if (param.get_data_type() == "std::string")
                {
                    return param.get_values()[0]; // Return single string value
                }
                else
                {
                    throw std::runtime_error("Requested data type does not match parameter data type.");
                }
            }
            std::cout << name << std::endl;
            throw std::runtime_error("Parameter not found");
        }

        bool get_bool(const std::string& name) const
        {
            auto it = parameters.find(name);
            if (it != parameters.end())
            {
                const parameter& param = it->second;

                if (param.get_data_type() == "bool")
                {
                    return param.get_values()[0] == "true";  // Return boolean
                }
                else
                {
                    throw std::runtime_error("Requested data type does not match parameter data type.");
                }
            }
            throw std::runtime_error("Parameter not found");
        }

        void set_bool(const std::string& name, bool value)
        {
            add_parameter(name, parameter(name, value));
        }


        void load_from_file()
        {
            std::ifstream file(default_filename);
            if (!file)
            {
                std::cerr << "File not found, creating default settings.txt" << std::endl;
                create_default_settings_file();
                file.open(default_filename);  
                if (!file)
                {
                    throw std::runtime_error("Unable to open settings.txt after creating it");
                }
            }

            std::string line;
            while (std::getline(file, line))
            {
                std::string name, data_type;
                std::istringstream iss(line);

                // Read parameter name and data type, then values based on the type
                if (iss >> name >> data_type)
                {
                    if (data_type == "double")
                    {
                        double value1;
                        if (iss >> value1)
                        {
                            add_parameter(name, parameter(name, value1));
                        }
                    }
                    else if (data_type == "int")
                    {
                        int value1;
                        if (iss >> value1)
                        {
                            add_parameter(name, parameter(name, value1));
                        }
                    }
                    else if (data_type == "std::string")
                    {
                        std::string value1;
                        if (iss >> value1)
                        {
                            add_parameter(name, parameter(name, value1));
                        }
                    }
                    else if (data_type == "bool")
                    {
                        std::string bool_str;
                        if (iss >> bool_str)
                        {
                            bool value = (bool_str == "true");
                            add_parameter(name, parameter(name, value));
                        }
                    }
                    else
                    {
                        std::cerr << "Unknown data type: " << data_type << " for parameter: " << name << std::endl;
                    }
                }
            }

            file.close();
        }


        void save_to_file() const
        {
            std::ofstream file(default_filename);
            if (!file)
            {
                throw std::runtime_error("Unable to open settings.txt for writing");
            }

            for (const auto& pair : parameters)
            {
                const parameter& param = pair.second;
                file << pair.first << " " << param.get_data_type() << " ";  // Output parameter name and data type

                if (param.get_data_type() == "double")
                {
                    // Save double values (up to 3 values)
                    for (const auto& value_str : param.get_values())
                    {
                        file << value_str << " ";
                    }
                }
                else if (param.get_data_type() == "int")
                {
                    // Save int values (up to 3 values)
                    for (const auto& value_str : param.get_values())
                    {
                        file << value_str << " ";
                    }
                }
                else if (param.get_data_type() == "std::string" || param.get_data_type() == "bool")
                {
                    file << param.get_values()[0] << " ";  // Save single string or bool value
                }
                file << "\n";  // Ensure a new line after each parameter
            }

            file.close();
}
          // Getters and setters
        double get_atom_radius() const { return get_double("atom_radius")[0]; }
        void set_atom_radius(double value) { add_parameter("atom_radius", parameter("atom_radius", value)); }

        double get_atom_mass() const { return get_double("atom_mass")[0]; }
        void set_atom_mass(double value) { add_parameter("atom_mass", parameter("atom_mass", value)); }

        int get_animation_step_duration() const { return get_int("animation_step_duration")[0]; }
        void set_animation_step_duration(int value) { add_parameter("animation_step_duration", parameter("animation_step_duration", value)); }

        int get_cubes_in_x() const { return get_int("cubes_in_x")[0]; }
        void set_cubes_in_x(int value) { add_parameter("cubes_in_x", parameter("cubes_in_x", value)); }

        int get_cubes_in_y() const { return get_int("cubes_in_y")[0]; }
        void set_cubes_in_y(int value) { add_parameter("cubes_in_y", parameter("cubes_in_y", value)); }

        int get_cubes_in_z() const { return get_int("cubes_in_z")[0]; }
        void set_cubes_in_z(int value) { add_parameter("cubes_in_z", parameter("cubes_in_z", value)); }

        double get_atom_spacing() const { return get_double("atom_spacing")[0]; }
        void set_atom_spacing(double value) { add_parameter("atom_spacing", parameter("atom_spacing", value)); }

        double get_energy_applied_to_impact_atom() const { return get_double("energy_applied_to_impact_atom")[0]; }
        void set_energy_applied_to_impact_atom(double value) { add_parameter("energy_applied_to_impact_atom", parameter("energy_applied_to_impact_atom", value)); }

        double get_impact_atom_x_offset() const { return get_double("impact_atom_x_offset")[0]; }
        void set_impact_atom_x_offset(double value) { add_parameter("impact_atom_x_offset", parameter("impact_atom_x_offset", value)); }

        double get_impact_atom_y_offset() const { return get_double("impact_atom_y_offset")[0]; }
        void set_impact_atom_y_offset(double value) { add_parameter("impact_atom_y_offset", parameter("impact_atom_y_offset", value)); }

        double get_impact_atom_z_offset() const { return get_double("impact_atom_z_offset")[0]; }
        void set_impact_atom_z_offset(double value) { add_parameter("impact_atom_z_offset", parameter("impact_atom_z_offset", value)); }

        double get_ev_to_j_per_mole() const { return get_double("ev_to_j_per_mole")[0]; }
        void set_ev_to_j_per_mole(double value) { add_parameter("ev_to_j_per_mole", parameter("ev_to_j_per_mole", value)); }

        double get_j_per_mole_to_ev() const { return get_double("j_per_mole_to_ev")[0]; }
        void set_j_per_mole_to_ev(double value) { add_parameter("j_per_mole_to_ev", parameter("j_per_mole_to_ev", value)); }

        double get_scaling() const { return get_double("scaling")[0]; }
        void set_scaling(double value) { add_parameter("scaling", parameter("scaling", value)); }

        double get_epsilon() const { return get_double("epsilon")[0]; }
        void set_epsilon(double value) { add_parameter("epsilon", parameter("epsilon", value)); }

        double get_sigma() const { return get_double("sigma")[0]; }
        void set_sigma(double value) { add_parameter("sigma", parameter("sigma", value)); }

        double get_rcutoff() const { return get_double("rcutoff")[0]; }
        void set_rcutoff(double value) { add_parameter("rcutoff", parameter("rcutoff", value)); }

        double get_velocity_scale() const { return get_double("velocity_scale")[0]; }
        void set_velocity_scale(double value) { add_parameter("velocity_scale", parameter("velocity_scale", value)); }

        int get_simulation_history_interval() const { return get_int("simulation_history_interval")[0]; }
        void set_simulation_history_interval(int value) { add_parameter("simulation_history_interval", parameter("simulation_history_interval", value)); }

        double get_simulation_timestep_size() const { return get_double("simulation_timestep_size")[0]; }
        void set_simulation_timestep_size(double value) { add_parameter("simulation_timestep_size", parameter("simulation_timestep_size", value)); }

        int get_simulation_total_timesteps() const { return get_int("simulation_total_timesteps")[0]; }
        void set_simulation_total_timesteps(int value) { add_parameter("simulation_total_timesteps", parameter("simulation_total_timesteps", value)); }
        
        std::string get_atom_mode() const 
        { 
            return get_string("atom_mode"); 
        }

        std::string get_atom_filename() const 
        { 
            return get_string("atom_filename"); 
        }
       
        bool get_add_impact_on() const { return get_bool("add_impact_on"); }
        void set_add_impact_on(bool value) { add_parameter("add_impact_on", parameter("add_impact_on", value)); }

        bool get_parallel_projection_on() const { return get_bool("parallel_projection_on"); }
        void set_parallel_projection_on(bool value) { add_parameter("parallel_projection_on", parameter("parallel_projection_on", value)); }

        bool get_animation_on() const { return get_bool("animation_on"); }
        void set_animation_on(bool value) { add_parameter("animation_on", parameter("animation_on", value)); }

        bool get_simulation_on() const { return get_bool("simulation_on"); }
        void set_simulation_on(bool value) { add_parameter("simulation_on", parameter("simulation_on", value)); }

        // Method to print all settings with dynamic values and proper format
        void print_all_settings() const
        {
            std::cout << "Settings:" << std::endl;  // Print title

            for (const auto& pair : parameters)
            {
                std::cout<< pair.first << " " << pair.second.get_data_type() << " ";
                
                for (const auto& value : pair.second.get_values())
                {
                    std::cout << value << " "; // Print all values, dynamically
                }

                std::cout << std::endl;
            }

            std::cout << " " << std::endl;
        }


};


#endif