#include "Structure.h"
#include <iostream>
#include <stdexcept>

namespace atoms {

    Structure::Structure(std::vector<Atom> atoms, std::string name = "Unstructured", std::string structure_type = "Unstructured", std::string atom_type = "Cu")
    {
        this->atoms = atoms;
        this->name = name + "_" + atom_type;
        this->structure_type = structure_type;
        this->atom_type = atom_type;
        this->box = Box(atoms);
        this->label_atoms();
    }

    Structure::Structure(int cubes_in_x, int cubes_in_y, int cubes_in_z, std::string name = "Fcc", std::string structure_type = "Fcc", Atom atom = Atom("Cu", 63.546, 1.28), 
    double atom_spacing = 3.61)
    {
        this->name = name + "_" + atom.type;
        this->structure_type = structure_type;
        this->atom_type = atom.type;
        this->atoms = generate_atoms_in_fcc_pattern(cubes_in_x, cubes_in_y, cubes_in_z, atom_spacing, atom.type, atom.mass, atom.radius);
        this->box = Box(atoms);
        this->label_atoms();
    }

    Structure::~Structure() 
    {
        atoms.clear();
        atom_pointers.clear();
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
        if (atoms_assigned_to_simulation)
        {
            std::vector<Atom> atoms;
            for (Atom* atom_pointer : atom_pointers)
            {
                atoms.push_back(*atom_pointer);
            }
            return atoms;
        }
        else
        {
            return atoms;
        }
    }

    Box Structure::get_box()
    {
        return box;
    }


    void Structure::assign_atoms_to_simulation(std::vector<Atom>& atom_receiver)
    {

        const std::size_t start_idx = atom_receiver.size();


        atom_receiver.insert(atom_receiver.end(),
                            std::make_move_iterator(atoms.begin()),
                            std::make_move_iterator(atoms.end()));

        atoms_assigned_to_simulation = true;


        atom_pointers.reserve(atom_pointers.size() + atoms.size());
        for (std::size_t i = start_idx; i < atom_receiver.size(); ++i)
        {
            atom_pointers.push_back(&atom_receiver[i]);
        }

    }

    void Structure::label_atoms()
    {
        for (Atom& atom : atoms)
        {
            atom.parent_structure = this->name;
        }
    }
    

}

