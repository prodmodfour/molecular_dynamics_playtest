#pragma once

#include <vector>
#include <string>
#include "Atom.h"
#include "../geometry/Box.h"

namespace atoms {

    class Structure 
    {
        public:
            Structure();
            ~Structure();

            void add_atom(Atom atom);
            void add_atoms(std::vector<Atom> atoms);

            void set_name(std::string name);
            void set_structure_type(std::string structure_type);
            void set_atom_type(std::string atom_type);

            std::vector<Atom> get_atoms();
            Box get_box();

            void assign_atoms_to_simulation();

        private:
            std::vector<Atom> atoms;
            std::vector<Atom*> atom_pointers;
            
            std::string name;
            std::string structure_type;
            std::string atom_type;

            bool atoms_assigned_to_simulation;
            Box box;


    };
}

