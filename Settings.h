
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
};

class Settings
{
    private:
        std::map<std::string, parameter> parameters;

    public:
        // Add a new parameter
        void add_parameter(const std::string& name, const parameter& parameter)
        {
            parameters[name] = parameter;
        }

        // Retrieve a parameter by name
        parameter get_parameter(const std::string& name) const
        {
            auto it = parameters.find(name);
            if (it != parameters.end())
            {
                return it->second;
            }
            throw std::runtime_error("Parameter not found");
        }

        // Load parameters from a file
        void load_from_file(const std::string& file_path)
        {
            std::ifstream file(file_path);
            if (!file)
            {
                throw std::runtime_error("Unable to open file");
            }

            std::string line;
            while (std::getline(file, line))
            {
                std::string name;
                double value1, value2, value3;
                std::istringstream iss(line);
                if (iss >> name >> value1 >> value2 >> value3)
                {
                    add_parameter(name, parameter(name, value1, value2, value3));
                }
            }
            file.close();
        }

        // Save parameters to a file
        void save_to_file(const std::string& file_path) const
        {
            std::ofstream file(file_path);
            if (!file)
            {
                throw std::runtime_error("Unable to open file for writing");
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
                file << "\n";  // Ensuring a new line is correctly inserted
            }
            file.close();
        }
};

#endif
