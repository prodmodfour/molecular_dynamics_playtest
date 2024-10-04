#ifndef __file_functions_h
#define __file_functions_h

#include <vector>
#include "Type_atom.h"
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "string_functions.h"



void write_atoms_to_file(std::vector<Type_atom> &all_atoms)
{
    std::ofstream file;
    file.open("block.xyz");

    file << all_atoms.size() << std::endl;
    for (int i = 0; i < all_atoms.size(); i++)
    {
        file << "Cu " << all_atoms[i].x << " " << all_atoms[i].y << " " << all_atoms[i].z << std::endl;
    }
}

std::vector<Type_atom> read_atoms_from_file(std::string filename)
{
    std::ifstream file;
    std::string line;
    std::vector<std::string> words;
    Type_atom atom;
    std::vector<Type_atom> all_atoms;

    file.open(filename);

    bool first_line = true;
    while(std::getline(file, line))
    {
        if(first_line)
        {
            first_line = false;
            continue;
        }

        words = split_sentence(line);
        if (words[0] == "Cu")
        {
            atom.x = std::stod(words[1]);
            atom.y = std::stod(words[2]);
            atom.z = std::stod(words[3]);
        }
        else
        {
            atom.x = std::stod(words[0]);
            atom.y = std::stod(words[1]);
            atom.z = std::stod(words[2]);
        }

        all_atoms.push_back(atom);


    }

    return all_atoms;

}



#endif