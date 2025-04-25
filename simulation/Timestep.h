#pragma once

#include "../atoms/Atom.h"
#include "../atoms/Structure.h" // Include Structure header
#include "Config.h"
#include <vector>
#include <string>
#include <map>
#include "StructureList.h"
#include <stdexcept> // For exceptions

namespace simulation {

class Timestep
{
    public:
        simulation::Config config;
        std::vector<atoms::Atom> atoms; // Holds copies of atoms from all structures for this timestep
        double kinetic_energy;
        double potential_energy;
        double time;
        // Removed StructureList - redundant if structures map holds the actual Structure objects
        // simulation::StructureList structure_list; 
        std::map<std::string, atoms::Structure> structures; // Map from structure name to Structure object

        // Constructor
        Timestep(simulation::Config config, std::vector<atoms::Atom> initial_atoms, double kinetic_energy, double potential_energy, double time)
            : config(config),
              atoms(std::move(initial_atoms)), // Use move semantics
              kinetic_energy(kinetic_energy),
              potential_energy(potential_energy),
              time(time),
              structures() // Initialize empty map
        {
            // If initial_atoms are provided, maybe create a default structure for them?
            // Or assume they are added via add_structure later.
        }

        // Default constructor
        Timestep()
            : config(),
              atoms(),
              kinetic_energy(0),
              potential_energy(0),
              time(0),
              structures()
        {
        }

        // Check if a structure with the given name exists
        bool structure_exists(const std::string& structure_name) const
        {
            return structures.count(structure_name) > 0;
        }

        // Add a fully formed structure
        void add_structure(atoms::Structure new_structure) // Pass by value (copy) or const reference? Consider ownership.
        {
            std::string name = new_structure.get_name(); // Use getter
            if (structure_exists(name)) {
                // Handle error: structure with this name already exists
                throw std::runtime_error("Structure with name '" + name + "' already exists in timestep.");
            } else {
                 // Add atoms from the new structure to the timestep's central atom list
                std::vector<atoms::Atom> structure_atoms = new_structure.get_atoms(); // Use getter
                atoms.insert(atoms.end(), 
                             std::make_move_iterator(structure_atoms.begin()), 
                             std::make_move_iterator(structure_atoms.end()));

                // Add the structure itself to the map (moves the structure if passed by value)
                structures[name] = std::move(new_structure); 
            }
        }

        // Add a single atom (assigns it to an 'unstructured' group)
        void add_atom(atoms::Atom atom)
        {
            std::string structure_name = atom.type + "_atoms"; // e.g., "Cu_atoms"
            std::string structure_type = "Unstructured";
            std::string atom_type = atom.type;

            if (!structure_exists(structure_name))
            {
                // Create a new, empty structure for this atom type if it doesn't exist
                std::vector<atoms::Atom> initial_atoms; // Empty vector
                // Use the correct constructor for Structure
                structures[structure_name] = atoms::Structure(initial_atoms, structure_name, structure_type, atom_type); 
            }
            
            // Add the atom to the appropriate structure
            atom.parent_structure = structure_name; // Label the atom
            structures[structure_name].add_atom(atom); // Add to the structure's list
            
            // Also add a copy of the atom to the central timestep atom list
            atoms.push_back(atom); 
        }

        // Method to get a structure by name (optional)
        atoms::Structure& get_structure(const std::string& name) {
            auto it = structures.find(name);
            if (it == structures.end()) {
                throw std::runtime_error("Structure with name '" + name + "' not found in timestep.");
            }
            return it->second;
        }

        const atoms::Structure& get_structure(const std::string& name) const {
             auto it = structures.find(name);
             if (it == structures.end()) {
                 throw std::runtime_error("Structure with name '" + name + "' not found in timestep.");
             }
             return it->second;
         }

};

} // namespace simulation