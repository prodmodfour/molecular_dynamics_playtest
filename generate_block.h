#include <thread>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

typedef struct 
  {
        double x, y, z;
  } Type_atom;

typedef struct 
{
    int x, y, z;
} block_dimensions;

void generate_corner_atom_row(std::vector<Type_atom>& atom_block, Type_atom first_atom, block_dimensions cubes_in, double atom_spacing);
void generate_corner_atom_xy_plane(std::vector<Type_atom>& atom_block, block_dimensions cubes_in, double atom_spacing);
void generate_corner_atom_xyz_block(std::vector<Type_atom> first_atom_xy_plane, block_dimensions cubes_in, double atom_spacing);
void generate_corner_atoms(std::vector<Type_atom>& atom_block, Type_atom first_atom, block_dimensions cubes_in, double atom_spacing);

void generate_centre_atom_row(Type_atom first_atom, block_dimensions cubes_in, double atom_spacing);
void generate_centre_atom_xy_plane(std::vector<Type_atom> first_atom_row, block_dimensions cubes_in, double atom_spacing);
void generate_centre_atom_xyz_block(std::vector<Type_atom> first_atom_xy_plane, block_dimensions cubes_in, double atom_spacing);
void generate_centre_atoms(Type_atom first_atom, block_dimensions cubes_in, double atom_spacing);

void print_atoms(std::vector<Type_atom> atoms)
{
    // Print all atoms
    for (int i = 0; i < atoms.size(); i++)
    {
        printf("Index %d x %f y %f z %f\n", i, atoms[i].x, atoms[i].y, atoms[i].z);
    }
}

std::vector<Type_atom> generate_block( int cubes_in_x, int cubes_in_y, int cubes_in_z)
{
    printf("Cubes in x %d \nCubes in y %d\nCubes in z %d\n", cubes_in_x, cubes_in_y, cubes_in_z);

    block_dimensions cubes_in;
    cubes_in.x = cubes_in_x;
    cubes_in.y = cubes_in_y;
    cubes_in.z = cubes_in_z;
    double atom_spacing = 3.61; //Angstroms

    Type_atom first_corner_atom;
    first_corner_atom.x = 0;
    first_corner_atom.y = 0;
    first_corner_atom.z = 0;
    std::vector<Type_atom> atom_block;
    generate_corner_atoms(&atom_block, first_corner_atom, cubes_in, atom_spacing);


    // Type_atom first_centre_atom;
    // first_centre_atom.x = atom_spacing / 2;
    // first_centre_atom.y = atom_spacing / 2;
    // first_centre_atom.z = atom_spacing / 2;
    // std::vector<Type_atom> centre_atoms = generate_centre_atoms(first_centre_atom, cubes_in, atom_spacing);



    return atom_block;
}

void generate_corner_atom_row(std::vector<Type_atom>& atom_block, Type_atom first_atom, block_dimensions cubes_in, double atom_spacing);
{

    Type_atom atom;
    atom.y = first_atom.y;
    atom.z = first_atom.z;
    for (int i = 0; i < (cubes_in.x + 1); i++)
    {
        atom.x = first_atom.x + (i * atom_spacing);
        atom_block.push_back(atom);
    }

}

void generate_corner_atom_xy_plane(std::vector<Type_atom>& atom_block, Type_atom first_atom, block_dimensions cubes_in, double atom_spacing);
{
    for (int i = 0; i < (cubes_in.y + 1); i++)
    {
        first_atom.y += (i * atom_spacing);
        generate_centre_atom_row(&atom_block, first_atom, cubes_in, atom_spacing )
    }
}

void generate_corner_atoms(std::vector<Type_atom>& atom_block, Type_atom first_atom, block_dimensions cubes_in, double atom_spacing)
{
    for (int i = 0; i < (cubes_in.z + 1); i++)
    {
        first_atom.z += (i * atom_spacing);
        generate_corner_atom_xy_plane(&atom_block, first_atom, cubes_in, atom_spacing );
    }

}