#include <thread>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

typedef struct 
  {
        double x, y, z;
  } Type_atoms;

void print_atoms(std::vector<Type_atoms> atoms)
{
    // Print all atoms
    for (int i = 0; i < atoms.size(); i++)
    {
        printf("Index %d x %f y %f z %f\n", i, atoms[i].x, atoms[i].y, atoms[i].z);
    }
}

std::vector<Type_atoms> generate_block( int cubes_in_x, int cubes_in_y, int cubes_in_z)
{
    std::vector<Type_atoms> atoms;
    printf("Cubes in x %d \nCubes in y %d\nCubes in z %d\n", cubes_in_x, cubes_in_y, cubes_in_z);

    double atom_spacing = 3.61; //Angstroms

    // Add first atom at origin
    int atom_counter = 1;
    Type_atoms atom;
    atom.x = 0;
    atom.y = 0;
    atom.z = 0;
    atoms.push_back(atom);

    // Complete first row of corner atoms
    int corner_atoms_in_x = cubes_in_x + 1;
    for (int i = 1; i < corner_atoms_in_x; i++)
    {
        atom.x = atoms[i - 1].x + atom_spacing;
        atom.y = 0;
        atom.z = 0;
        atoms.push_back(atom);
        atom_counter++;
    }

    // Complete first plane of corner atoms
    for (int i = 1; i < (cubes_in_y + 1); i++)
    {
        for (int j = 0; j < corner_atoms_in_x; j++)
        {
            atom.x = atoms[j].x;
            atom.y = atoms[j].y + (i * atom_spacing);
            atom.z = 0;
            atoms.push_back(atom);
            atom_counter++;
        }
    }

    // Complete block of corner atoms
    int atoms_in_one_plane = corner_atoms_in_x * (cubes_in_y + 1);
    for (int i = 1; i < (cubes_in_z + 1); i++)
    {
        for (int j = 0; j < atoms_in_one_plane; j++)
        {
            atom.x = atoms[j].x;
            atom.y = atoms[j].y;
            atom.z = atoms[j].z + (i * atom_spacing);
            atoms.push_back(atom);
            atom_counter++;
        }
    }

    // Add in first centre atom
    atom.x = atom_spacing / 2;
    atom.y = atom_spacing / 2;
    atom.z = atom_spacing / 2;
    atoms.push_back(atom);
    atom_counter++;

    // // Complete first row of centre atoms
    for (int i = 1; i < cubes_in_x; i++)
    {
        atom.x = atoms[atom_counter - 1].x + atom_spacing;
        atom.y = atoms[atom_counter - 1].y;
        atom.z = atoms[atom_counter - 1].z;
        atoms.push_back(atom);
        atom_counter++;
    }

    // Complete first plane of centre atoms
    
    for (int i = 1; i < cubes_in_y; i++)
    {
        int start = atom_counter - (i * cubes_in_x);
        int end = atom_counter;
        for (int j = start; j < end; j++)
        {
            atom.x = atoms[j].x;
            atom.y = atoms[j].y + (i * atom_spacing);
            atom.z = atoms[j].z;
            atoms.push_back(atom);
            atom_counter++;
        }
    }

    // // Complete block of corner atoms
    for (int i = 1; i < cubes_in_z; i++)
    {
        int start = (atom_counter - (i * (cubes_in_x * cubes_in_y)));
        int end = atom_counter;
        for (int j = start; j < end; j++)
        {
            atom.x = atoms[j].x;
            atom.y = atoms[j].y ;
            atom.z = atoms[j].z + (i * atom_spacing);
            atoms.push_back(atom);
            atom_counter++;
        }
    }

    return atoms;
}
