#include "Structure.h"
#include "../geometry/Box.h" // Included for geometry::Box
#include "atom_generation_functions.h" // Included for generate_atoms_in_fcc_pattern
#include <vector>
#include <string>
#include <stdexcept> // For potential exceptions
#include <iterator> // For std::make_move_iterator if needed (though changing approach)
#include <iostream> // Included for potential debugging if needed

namespace atoms {

    // Default constructor (as declared in .h)
    Structure::Structure() : name("Default"), structure_type("None"), atom_type("None"), box(), atoms_assigned_to_simulation(false) {
        // Initialize box if necessary, e.g., to an empty box
        // box = geometry::Box(); // Assuming geometry::Box has a default constructor
    }

    // Constructor from vector of atoms (matching declaration in .h)
    Structure::Structure(std::vector<Atom> input_atoms, std::string structure_name, std::string type, std::string atom_element_type)
        : atoms(std::move(input_atoms)), // Use move semantics for efficiency
          name(structure_name), 
          structure_type(type), 
          atom_type(atom_element_type), 
          box(this->atoms), // Initialize box using the atoms vector
          atoms_assigned_to_simulation(false) 
    {
        // If name needs combining, do it here
        // this->name = structure_name + "_" + atom_element_type; 
        this->label_atoms();
    }

    // Constructor for FCC pattern (matching declaration in .h)
    Structure::Structure(int nx, int ny, int nz, std::string structure_name, std::string type, atoms::Atom basis_atom, double lattice_constant)
        : name(structure_name), 
          structure_type(type), 
          atom_type(basis_atom.type), 
          atoms_assigned_to_simulation(false)
    {
        // If name needs combining, do it here
        // this->name = structure_name + "_" + basis_atom.type; 
        this->atoms = generate_atoms_in_fcc_pattern(nx, ny, nz, lattice_constant, basis_atom.type, basis_atom.mass, basis_atom.radius);
        this->box = geometry::Box(this->atoms); // Initialize box after atoms are generated
        this->label_atoms();
    }

    Structure::~Structure() 
    {
        // Vectors will clear themselves automatically
        // atoms.clear(); 
        // atom_pointers.clear(); 
    }

    void Structure::add_atom(Atom new_atom)
    {
        // Ensure atom knows its parent structure
        new_atom.parent_structure = this->name;
        atoms.push_back(std::move(new_atom)); // Use move semantics
        // Potentially update the box? Depends on Box class design.
        // box.update_bounds(atoms); 
    }

    void Structure::add_atoms(std::vector<Atom> new_atoms)
    {
        atoms.reserve(atoms.size() + new_atoms.size()); // Reserve space for efficiency
        for (Atom& atom : new_atoms)
        {
            // Ensure atom knows its parent structure
            atom.parent_structure = this->name; 
            atoms.push_back(std::move(atom)); // Use move semantics
        }
         // Potentially update the box? Depends on Box class design.
        // box.update_bounds(atoms);
    }

    void Structure::set_name(std::string new_name)
    {
        this->name = new_name;
        // Update parent structure name for existing atoms
        this->label_atoms(); 
    }

    void Structure::set_structure_type(std::string new_structure_type)
    {
        this->structure_type = new_structure_type;
    }

    void Structure::set_atom_type(std::string new_atom_type)
    {
        this->atom_type = new_atom_type;
        // Note: This doesn't change the type of existing atoms. 
        // Might need logic to update existing atoms if that's the intent.
    }

    // Definition for get_atoms (const)
    std::vector<Atom> Structure::get_atoms() const
    {
        // Simply return a copy of the internal atoms vector.
        // The logic involving atom_pointers and atoms_assigned_to_simulation seemed overly complex
        // and potentially problematic. If pointers are needed, use get_atom_pointers().
        return atoms; 
    }
    
    // Definition for get_atom_pointers (const)
    const std::vector<Atom*>& Structure::get_atom_pointers() const {
         if (!atoms_assigned_to_simulation) {
             // Maybe throw an error or log a warning if pointers are requested before assignment?
             // std::cerr << "Warning: Requesting atom pointers before assign_atoms_to_simulation is called." << std::endl;
             // Or perhaps populate the pointers here on first request?
             // This requires atom_pointers and atoms_assigned_to_simulation to be mutable, 
             // or this method cannot be const. Let's stick to the simple case first.
             // For now, return the potentially empty vector.
         }
         return atom_pointers;
     }


    // Definition for get_name (const)
    std::string Structure::get_name() const {
        return name;
    }

    // Definition for get_structure_type (const)
    std::string Structure::get_structure_type() const {
        return structure_type;
    }
    
    // Definition for get_atom_type (const)
    std::string Structure::get_atom_type() const {
        return atom_type;
    }


    // Definition for get_box (const) - Ensure correct signature
    geometry::Box Structure::get_box() const
    {
        return box;
    }


    // Definition for assign_atoms_to_simulation (matching header)
    // This function populates the provided vector with pointers to the atoms owned by this Structure.
    void Structure::assign_atoms_to_simulation(std::vector<Atom*>& simulation_atom_pointers)
    {
        // Clear any old pointers from previous assignments for this structure
        atom_pointers.clear(); 
        atom_pointers.reserve(atoms.size()); // Reserve space

        // Add pointers to our internal atoms to the simulation vector AND our internal pointer vector
        for (Atom& atom : atoms) {
            Atom* atom_ptr = &atom;
            simulation_atom_pointers.push_back(atom_ptr);
            atom_pointers.push_back(atom_ptr); // Keep track internally too
        }
        
        atoms_assigned_to_simulation = true;
    }

    // Definition for label_atoms
    void Structure::label_atoms()
    {
        for (Atom& atom : atoms)
        {
            atom.parent_structure = this->name;
        }
    }
    

} // namespace atoms
