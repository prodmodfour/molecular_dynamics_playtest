#ifndef __Settings_h
#define __Settings_h
#include "Type_atom.h"
#include "file_functions.h"
#include "string_functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <stdexcept>

template<typename T>
std::string to_string(T value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

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
        // Default constructor
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
        const std::string default_filename = "settings.txt"; 


        void add_parameter(const std::string& name, const parameter& parameter)
        {
            parameters[name] = parameter;
        }


        void create_default_settings_file()
        {
            std::ofstream file(default_filename);
            if (!file)
            {
                throw std::runtime_error("Unable to create default settings.txt");
            }

            // Add default parameters with their data types
            file << "parameter1 double 1.0 2.0 3.0\n";
            file << "parameter2 int 1 2 3\n";
            file << "parameter3 string hello world test\n";
            
            file.close();
}

    public:

        parameter get_parameter(const std::string& name) const
        {
            auto it = parameters.find(name);
            if (it != parameters.end())
            {
                return it->second;
            }
            throw std::runtime_error("Parameter not found");
        }


   void load_from_file()
{
    std::ifstream file(default_filename);
    if (!file)
    {
        std::cerr << "File not found, creating default settings.txt" << std::endl;
        create_default_settings_file();
        file.open(default_filename);  // Try opening the newly created file
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

        // Read parameter name and data type
        if (iss >> name >> data_type)
        {
            if (data_type == "double")
            {
                double value1, value2, value3;
                if (iss >> value1 >> value2 >> value3)
                {
                    add_parameter(name, parameter(name, value1, value2, value3));
                }
            }
            else if (data_type == "int")
            {
                int value1, value2, value3;
                if (iss >> value1 >> value2 >> value3)
                {
                    add_parameter(name, parameter(name, value1, value2, value3));
                }
            }
            else if (data_type == "string")
            {
                std::string value1, value2, value3;
                if (iss >> value1 >> value2 >> value3)
                {
                    add_parameter(name, parameter(name, value1, value2, value3));
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
                const parameter& parameter = pair.second;
                file << pair.first << " "; // Output the parameter name
                double* values = parameter;
                for (int i = 0; i < 3; ++i)
                {
                    file << values[i] << " ";
                }
                file << "\\n";  // Ensuring a new line is correctly inserted
            }
            file.close();
        }

        // Method to print all settings with dynamic values and proper format
        void print_all_settings() const
        {
            std::cout << "Settings:" << std::endl;  // Print title

            for (const auto& pair : parameters)
            {
                std::cout << "Parameter: " << pair.first << " | Data Type: " << pair.second.get_data_type() << " | Values: ";
                
                for (const auto& value : pair.second.get_values())
                {
                    std::cout << value << " "; // Print all values, dynamically
                }

                std::cout << std::endl;
            }
        }
};


#endif