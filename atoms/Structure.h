#pragma once

#include <vector>
#include <string>
#include "Atom.h"
#include "../geometry/Box.h" // Included

// Forward declaration can sometimes help, though include is better
// namespace geometry { class Box; }

namespace atoms {

    class Structure 
    {
        public:
            // Constructors
            Structure(); // Default
            // Added based on errors C2440/C2511 in Timestep.h/Structure.cpp
            Structure(std::vector<Atom> atoms, std::string name, std::string structure_type, std::string atom_type);
            // Added based on errors C2661/C2511 in test_simulation_runs.cpp/Structure.cpp (FCC pattern?)
            Structure(int nx, int ny, int nz, std::string name, std::string structure_type, atoms::Atom basis_atom, double lattice_constant);
            
            ~Structure();

            void add_atom(Atom atom);
            void add_atoms(std::vector<Atom> atoms);

            void set_name(std::string name);
            void set_structure_type(std::string structure_type);
            void set_atom_type(std::string atom_type);

            // Getters
            std::vector<Atom> get_atoms() const; // Mark as const
            const std::vector<Atom*>& get_atom_pointers() const; // Getter for pointers, return const ref
            std::string get_name() const; // Added getter, mark as const
            std::string get_structure_type() const; // Added getter, mark as const
            std::string get_atom_type() const; // Added getter, mark as const
            geometry::Box get_box() const; // Mark as const

            void assign_atoms_to_simulation(std::vector<Atom*>& simulation_atom_pointers); // Modified to take pointer vector

            void label_atoms();

        private:
            std::vector<Atom> atoms;
            std::vector<Atom*> atom_pointers; // Pointers to atoms in simulation?
            
            std::string name;
            std::string structure_type;
            std::string atom_type;

            geometry::Box box; // Declaration seems fine

            bool atoms_assigned_to_simulation = false; // Added based on error C2065
    };
}
