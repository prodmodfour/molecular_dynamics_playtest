#pragma once
#include <vector>
#include "Atom.h"

namespace atoms {

std::vector<atoms::Atom> generate_atoms_in_fcc_pattern(int cubes_in_x, int cubes_in_y, int cubes_in_z, double atom_spacing, 
std::string type, double mass, double radius);

}