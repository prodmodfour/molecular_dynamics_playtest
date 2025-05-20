#pragma once

#include "../atoms/Atom.h"
#include "Config.h"
#include <vector>
#include <string>
#include <map>
#include "StructureList.h"
#include "../atoms/Structure.h"

namespace simulation {

typedef struct Timestep
{
    simulation::Config config;
    std::vector<atoms::Atom> atoms;
    double kinetic_energy;
    double potential_energy;
    double time;
    simulation::StructureList structure_list;
    std::map<std::string, atoms::Structure> structures;

    Timestep(simulation::Config config, std::vector<atoms::Atom> atoms, double kinetic_energy, double potential_energy, double time);
    Timestep();

    void add_structure(atoms::Structure structure);
    void add_atom(atoms::Atom atom);
} Timestep;

} // namespace simulation