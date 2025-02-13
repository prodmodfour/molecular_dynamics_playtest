#pragma once
#include <vector>
#include "Atom.h"

namespace atoms {

    std::vector<Atom> generate_atoms_in_fcc_pattern(int cubes_in_x, int cubes_in_y, int cubes_in_z, double atom_spacing, std::string type, double mass, double radius)
{
    std::vector<Atom> crystal;

    int x_cubes = cubes_in_x;
    int y_cubes = cubes_in_y;
    int z_cubes = cubes_in_z;
    double atom_spacing = atom_spacing;

    // FCC basis vectors
    std::vector<std::vector<double>> basis = {
        {0.0, 0.0, 0.0},
        {0.0, 0.5, 0.5},
        {0.5, 0.0, 0.5},
        {0.5, 0.5, 0.0}
    };

    // Generate crystal structure
    for (int ix = 0; ix < x_cubes; ix++) {
        for (int iy = 0; iy < y_cubes; iy++) {
            for (int iz = 0; iz < z_cubes; iz++) {
                for (const auto& b : basis) {
                    Atom atom(type, mass, radius);
                    atom.x = (ix + b[0]) * atom_spacing;
                    atom.y = (iy + b[1]) * atom_spacing;
                    atom.z = (iz + b[2]) * atom_spacing;

                    // Add atom to crystal
                    crystal.push_back(atom);
                }
            }
        }
    }


    return crystal;
}

}