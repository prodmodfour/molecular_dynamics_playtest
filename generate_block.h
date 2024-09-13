#include <thread>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>

typedef struct 
  {
        double x, y, z;
  } Type_atoms;

int determine_number_atoms(int cubes_in_x, int cubes_in_y, int cubes_in_z)
{
    // A unit cube has four corner atoms with one centre atom. 
    int corner_atoms_in_x = cubes_in_x + 1;
    int atoms_in_one_plane = corner_atoms_in_x * (cubes_in_y + 1);
    int total_corner_atoms = atoms_in_one_plane * (cubes_in_z + 1);
    
    int total_centre_atoms = cubes_in_x * cubes_in_y * cubes_in_z;
    int total_atoms = total_corner_atoms + total_centre_atoms;
    // We then add space at the end for an impact atom
    total_atoms++;
    return total_atoms;

}
void generate_block(std::vector<Type_atoms> atoms, int cubes_in_x, int cubes_in_y, int cubes_in_z)
{

    printf("Cubes in x %d \nCubes in y %d\nCubes in z %d\n", cubes_in_x, cubes_in_y, cubes_in_z);

    double atom_spacing = 3.61; //Angstroms

    // Add first atom at origin
    int atom_counter = 1;
    Type_atoms atom;
    atom.x = 0;
    atom.y = 0;
    atom.z = 0;

    // Complete first row of corner atoms
    int corner_atoms_in_x = cubes_in_x + 1;
    for (int i = 1; i < corner_atoms_in_x; i++)
    {
        atoms[i].x = atoms[i - 1].x + atom_spacing;
        atoms[i].y = 0;
        atoms[i].z = 0;
        atom_counter++;
    }

    // Complete first plane of corner atoms
    for (int i = 1; i < (cubes_in_y + 1); i++)
    {
        for (int j = 0; j < corner_atoms_in_x; j++)
        {
            atoms[atom_counter].x = atoms[j].x;
            atoms[atom_counter].y = atoms[j].y + (i * atom_spacing);
            atoms[atom_counter].z = 0;
            atom_counter++;
        }
    }

    // Complete block of corner atoms
    int atoms_in_one_plane = corner_atoms_in_x * (cubes_in_y + 1);
    for (int i = 1; i < (cubes_in_z + 1); i++)
    {
        for (int j = 0; j < atoms_in_one_plane; j++)
        {
            atoms[atom_counter].x = atoms[j].x;
            atoms[atom_counter].y = atoms[j].y;
            atoms[atom_counter].z = atoms[j].z + (i * atom_spacing);
            atom_counter++;
        }
    }

    // Add in first centre atom
    atoms[atom_counter].x = atom_spacing / 2;
    atoms[atom_counter].y = atom_spacing / 2;
    atoms[atom_counter].z = atom_spacing / 2;
    atom_counter++;

    // // Complete first row of centre atoms
    for (int i = 1; i < cubes_in_x; i++)
    {
        atoms[atom_counter].x = atoms[atom_counter - 1].x + atom_spacing;
        atoms[atom_counter].y = atoms[atom_counter - 1].y;
        atoms[atom_counter].z = atoms[atom_counter - 1].z;
        atom_counter++;
    }

    // Complete first plane of centre atoms
    
    for (int i = 1; i < cubes_in_y; i++)
    {
        int start = atom_counter - (i * cubes_in_x);
        int end = atom_counter;
        for (int j = start; j < end; j++)
        {
            atoms[atom_counter].x = atoms[j].x;
            atoms[atom_counter].y = atoms[j].y + (i * atom_spacing);
            atoms[atom_counter].z = atoms[j].z;
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
            atoms[atom_counter].x = atoms[j].x;
            atoms[atom_counter].y = atoms[j].y ;
            atoms[atom_counter].z = atoms[j].z + (i * atom_spacing);
            atom_counter++;
        }
    }


}
