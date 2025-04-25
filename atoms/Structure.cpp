#include "Structure.h"
#include <iostream>
#include <stdexcept>
#include "../geometry/Box.h"
#include "atom_generation_functions.h"

namespace atoms {

    Structure::Structure()
    {
        // Nothing
    }

    Structure::Structure(std::vector<Atom> atoms, std::string name, std::string structure_type, std::string atom_type)
    {
        this->atoms = atoms;
        this->name = name + "_" + atom_type;
        this->structure_type = structure_type;
        this->atom_type = atom_type;
        this->box = geometry::Box(atoms);
        this->label_atoms();
    }

    Structure::Structure(int cubes_in_x, int cubes_in_y, int cubes_in_z, std::string name, std::string structure_type , Atom atom, 
            double atom_spacing)
    {
        this->name = name + "_" + atom.type;
        this->structure_type = structure_type;
        this->atom_type = atom.type;
        this->atoms = generate_atoms_in_fcc_pattern(cubes_in_x, cubes_in_y, cubes_in_z, atom_spacing, atom.type, atom.mass, atom.radius);
        this->box = geometry::Box(atoms);
        this->label_atoms();
    }

    Structure::~Structure() 
    {
        atoms.clear();
    }

    void Structure::add_atom(Atom new_atom)
    {
        if (this->name == new_atom.parent_structure)
        {
            atoms.push_back(new_atom);
        }
        else
        {
            new_atom.parent_structure = this->name;
            atoms.push_back(new_atom);
        }


    }

    void Structure::add_atoms(std::vector<Atom> new_atoms)
    {
        for (Atom& atom : new_atoms)
        {
            add_atom(atom);
        }
    }

    void Structure::set_name(std::string name)
    {
        this->name = name;
    }

    void Structure::set_structure_type(std::string structure_type)
    {
        this->structure_type = structure_type;
    }

    void Structure::set_atom_type(std::string atom_type)
    {
        this->atom_type = atom_type;
    }

    std::vector<Atom> Structure::get_atoms()
    {
        return atoms;
    }

    void Structure::label_atoms()
    {
        for (Atom& atom : atoms)
        {
            atom.parent_structure = this->name;
        }
    }
    

}

