#include "StructureList.h"
#include <vector> // Ensure vector is included
#include <string> // Ensure string is included
#include <algorithm> // Needed for std::find, std::remove, std::distance
#include <iostream> // Needed for std::cout, std::endl

namespace simulation {

StructureList::StructureList()
{
    // Default constructor initializes structure_names automatically
}

void StructureList::add_structure(atoms::Structure structure) // Consider passing by const reference
{
    // Check if the structure name already exists
    std::string name = structure.get_name(); // Use getter
    if (!structure_exists(name)) // Use the class's own method
    {
        structure_names.push_back(name);
    }
}

void StructureList::remove_structure(std::string structure_name)
{
    // std::remove shifts elements, erase actually removes them
    structure_names.erase(std::remove(structure_names.begin(), structure_names.end(), structure_name), structure_names.end());
}

void StructureList::rename_structure(std::string old_name, std::string new_name)
{
    auto it = std::find(structure_names.begin(), structure_names.end(), old_name);
    // Check if the old name exists
    if (it != structure_names.end())
    {
        // Check if the new name already exists (optional, depends on desired behavior)
        if (!structure_exists(new_name)) {
            // Replace the old name with the new name
            *it = new_name;
        } else {
             std::cerr << "Error: New structure name '" << new_name << "' already exists." << std::endl;
             // Or handle differently, e.g., throw an exception
        }
       
    }
    else
    {
        std::cerr << "Error: Old structure name '" << old_name << "' not found." << std::endl;
         // Or handle differently, e.g., throw an exception
    }
}

// Correct return type to match header
std::vector<std::string> StructureList::get_all_structure_names()
{
    return structure_names;
}

void StructureList::print_all_structure_names()
{
    for (const std::string& name : structure_names) // Use const reference
    {
        std::cout << name << std::endl;
    }
}

bool StructureList::structure_exists(std::string structure_name)
{
    return std::find(structure_names.begin(), structure_names.end(), structure_name) != structure_names.end();
}


} // namespace simulation
