#include "StructureList.h"
#include "../atoms/Structure.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

namespace simulation {

StructureList::StructureList()
{
    structure_names = std::vector<std::string>();
}

void StructureList::add_structure(atoms::Structure structure)
{
    // Check if the structure name already exists
    if (std::find(structure_names.begin(), structure_names.end(), structure.name) == structure_names.end())
    {
        structure_names.push_back(structure.name);
    }
}

void StructureList::remove_structure(std::string structure_name)
{
    structure_names.erase(std::remove(structure_names.begin(), structure_names.end(), structure_name), structure_names.end());
}

void StructureList::rename_structure(std::string old_name, std::string new_name)
{
    // Check if the old name exists
    if (std::find(structure_names.begin(), structure_names.end(), old_name) != structure_names.end())
    {
        // Find the index of the old name
        size_t index = std::distance(structure_names.begin(), std::find(structure_names.begin(), structure_names.end(), old_name));
        // Replace the old name with the new name
        structure_names[index] = new_name;
    }
    else
    {
        std::cout << "Structure name not found" << std::endl;
    }
}

std::vector<std::string> StructureList::get_all_structure_names()
{
    return structure_names;
}

void StructureList::print_all_structure_names()
{
    for (std::string name : structure_names)
    {
        std::cout << name << std::endl;
    }
}

bool StructureList::structure_exists(std::string structure_name)
{
    return std::find(structure_names.begin(), structure_names.end(), structure_name) != structure_names.end();
}


}