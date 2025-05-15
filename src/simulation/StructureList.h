#pragma once
#include "../atoms/Structure.h"
#include <vector>
#include <string>


namespace simulation {

class StructureList
{
    public:
        StructureList();
        

        void add_structure(atoms::Structure structure);
        void remove_structure(std::string structure_name);
        void rename_structure(std::string old_name, std::string new_name);

        std::vector<std::string> get_all_structure_names();
        void print_all_structure_names();
        bool structure_exists(std::string structure_name);
    
    private:
        std::vector<std::string> structure_names;
};

}