#include "atom_generation_functions.h"

namespace atoms {

std::vector<atoms::Atom> generate_atoms_in_fcc_pattern(int cubes_in_x, int cubes_in_y, int cubes_in_z, double atom_spacing, std::string type, double mass, double radius)
{
    std::vector<atoms::Atom> crystal;

    std::vector<std::vector<double>> basis = {
        {0.0, 0.0, 0.0},
        {0.0, 0.5, 0.5},
        {0.5, 0.0, 0.5},
        {0.5, 0.5, 0.0}
    };

    for (int ix = 0; ix < cubes_in_x; ix++) {
        for (int iy = 0; iy < cubes_in_y; iy++) {
            for (int iz = 0; iz < cubes_in_z; iz++) {
                for (const auto& b : basis) {
                    atoms::Atom atom(type, mass, radius);
                    atom.x = (ix + b[0]) * atom_spacing;
                    atom.y = (iy + b[1]) * atom_spacing;
                    atom.z = (iz + b[2]) * atom_spacing;


                    crystal.push_back(atom);
                }
            }
        }
    }

    return crystal;
}

} 