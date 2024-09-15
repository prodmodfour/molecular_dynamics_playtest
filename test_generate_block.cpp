#include "generate_atoms.h"

int main(int argc, char *argv[])
{
    if(argc != 4)
    {
        printf("Incorrect number of input arguments.\n");
        printf("Please input 3 integers (Cubes in x, Cubes in y, Cubes in z)");
        return 0;
    }
    block_dimensions cubes_in;

    cubes_in.x = std::stoi(argv[1]);
    cubes_in.y = std::stoi(argv[2]); 
    cubes_in.z = std::stoi(argv[3]); 

    std::vector<Type_atom> atom_block = generate_atom_block(cubes_in);
    print_atoms(atom_block);

    return 1;
}