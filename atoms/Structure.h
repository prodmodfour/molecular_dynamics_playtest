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
            Structure(std::vector<Atom> atoms, std::string name = "Unstructured", std::string structure_type = "Unstructured", std::string atom_type = "Cu");

            Structure(int cubes_in_x, int cubes_in_y, int cubes_in_z, std::string name = "Fcc", std::string structure_type = "Fcc", Atom atom = Atom("Cu", 63.546, 1.28), 
    double atom_spacing = 3.61);


            void add_atom(Atom atom);
            void add_atoms(std::vector<Atom> atoms);

            void set_name(std::string name);
            void set_structure_type(std::string structure_type);
            void set_atom_type(std::string atom_type);

            std::vector<Atom> get_atoms();


            void label_atoms();
            std::vector<Atom> atoms;
            
            std::string name;
            std::string structure_type;
            std::string atom_type;

            geometry::Box box;




    };
}

