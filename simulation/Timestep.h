#pragma once

#include "../atoms/Atom.h"
#include "Config.h"
#include <vector>
#include <string>
#include <map>
#include "StructureList.h"
#include "../atoms/Structure.h"

namespace simulation {

class Timestep
{
    public:
        simulation::Config config;
        std::vector<atoms::Atom> atoms;
        double kinetic_energy;
        double potential_energy;
        double time;
        simulation::StructureList structure_list;
        std::map<std::string, atoms::Structure> structures;

        Timestep(simulation::Config config, std::vector<atoms::Atom> atoms, double kinetic_energy, double potential_energy, double time)
        {
            this->config = config;
            this->atoms = atoms;
            this->kinetic_energy = kinetic_energy;
            this->potential_energy = potential_energy;
            this->time = time;
            this->structure_list = simulation::StructureList();
            this->structures = std::map<std::string, atoms::Structure>();
        }

        Timestep()
        {
            this->config = simulation::Config();
            this->atoms = std::vector<atoms::Atom>();
            this->kinetic_energy = 0;
            this->potential_energy = 0;
            this->time = 0;
            this->structure_list = simulation::StructureList();
            this->structures = std::map<std::string, atoms::Structure>();
        }

        void add_structure(atoms::Structure structure)
        {
            this->structures[structure.name] = structure;
            this->structure_list.add_structure(structure);

            // Add the atoms to the atoms vector
            for (atoms::Atom atom : structure.atoms)
            {
                this->atoms.push_back(atom);
            }
        }

        void add_atom(atoms::Atom atom)
        {
            std::string structure_name = atom.type + "_atoms";
            std::string structure_type = "Unstructured";
            atom.parent_structure = structure_name;

            if (!structure_list.structure_exists(structure_name))
            {
                std::vector<atoms::Atom> new_atoms;
                new_atoms.push_back(atom);
                add_structure(atoms::Structure(new_atoms, structure_name, structure_type, atom.type));


            }
            else
            {
                this->structures[structure_name].add_atom(atom);
                this->atoms.push_back(atom);
            }

        }


};

} // namespace simulation