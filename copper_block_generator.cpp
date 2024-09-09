#include <thread>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>



int main(int argc, char *argv[])
{
    // There are 3 arguments to be passed to the program
    // x, y, and z
    // These are the number of unit cubes in the x, y and z direction

    if (argc != 4)
    {
        printf("Incorrect number of arguments. Please enter 3 integers (Number of unit cubes in the X, Y and Z directions)\n");
        return 0;
    }

    int cubes_in_x = std::stoi(argv[1]);
    int cubes_in_y = std::stoi(argv[2]);
    int cubes_in_z = std::stoi(argv[3]);



    printf("Cubes in x %d \nCubes in y %d\nCubes in z %d\n", cubes_in_x, cubes_in_y, cubes_in_z);
    // A unit cube has four corner atoms with one centre atom. 
    int corner_atoms_in_x = cubes_in_x + 1;
    int atoms_in_one_plane = corner_atoms_in_x * (cubes_in_y + 1);
    int total_corner_atoms = atoms_in_one_plane * (cubes_in_z + 1);
    
    int total_centre_atoms = cubes_in_x * cubes_in_y * cubes_in_z;
    int total_atoms = total_corner_atoms + total_centre_atoms;
    printf("Total Atoms %d\n", total_atoms);

    

    typedef struct {
        double x, y, z;
    } Type_atoms;
    Type_atoms *Atoms = new Type_atoms[total_atoms];

    double atom_spacing = 3.61; //Angstroms

    // Add first atom at origin
    int atom_counter = 1;
    Atoms[0].x = 0;
    Atoms[0].y = 0;
    Atoms[0].z = 0;

    // Complete first row of corner atoms
    for (int i = 1; i < corner_atoms_in_x; i++)
    {
        Atoms[i].x = Atoms[i - 1].x + atom_spacing;
        Atoms[i].y = 0;
        Atoms[i].z = 0;
        atom_counter++;
    }

    // Complete first plane of corner atoms
    for (int i = 1; i < (cubes_in_y + 1); i++)
    {
        for (int j = 0; j < corner_atoms_in_x; j++)
        {
            Atoms[atom_counter].x = Atoms[j].x;
            Atoms[atom_counter].y = Atoms[j].y + (i * atom_spacing);
            Atoms[atom_counter].z = 0;
            atom_counter++;
        }
    }

    // Complete block of corner atoms
    for (int i = 1; i < (cubes_in_z + 1); i++)
    {
        for (int j = 0; j < atoms_in_one_plane; j++)
        {
            Atoms[atom_counter].x = Atoms[j].x;
            Atoms[atom_counter].y = Atoms[j].y;
            Atoms[atom_counter].z = Atoms[j].z + (i * atom_spacing);
            atom_counter++;
        }
    }

    // Add in first centre atom
    Atoms[atom_counter].x = atom_spacing / 2;
    Atoms[atom_counter].y = atom_spacing / 2;
    Atoms[atom_counter].z = atom_spacing / 2;
    atom_counter++;

    // // Complete first row of centre atoms
    for (int i = 1; i < cubes_in_x; i++)
    {
        Atoms[atom_counter].x = Atoms[atom_counter - 1].x + atom_spacing;
        Atoms[atom_counter].y = Atoms[atom_counter - 1].y;
        Atoms[atom_counter].z = Atoms[atom_counter - 1].z;
        atom_counter++;
    }

    // Complete first plane of centre atoms
    
    for (int i = 1; i < cubes_in_y; i++)
    {
        int start = atom_counter - (i * cubes_in_x);
        int end = atom_counter;
        for (int j = start; j < end; j++)
        {
            Atoms[atom_counter].x = Atoms[j].x;
            Atoms[atom_counter].y = Atoms[j].y + (i * atom_spacing);
            Atoms[atom_counter].z = Atoms[j].z;
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
            Atoms[atom_counter].x = Atoms[j].x;
            Atoms[atom_counter].y = Atoms[j].y ;
            Atoms[atom_counter].z = Atoms[j].z + (i * atom_spacing);
            atom_counter++;
        }
    }


    // Print all atoms
    for (int i = 0; i < atom_counter; i++)
    {
        printf("Index %d x %f y %f z %f\n", i, Atoms[i].x, Atoms[i].y, Atoms[i].z);
    }







}
