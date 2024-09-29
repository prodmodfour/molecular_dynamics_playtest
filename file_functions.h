#ifndef __file_functions_h
#define __file_functions_h

#include <vector>
#include "Type_atom.h"
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

std::vector<std::string> split_sentence(std::string sen);

void write_atoms_to_file(std::vector<Type_atom> &all_atoms)
{
    std::ofstream file;
    file.open("block.xyz");

    file << all_atoms.size() << std::endl;
    for (int i = 0; i < all_atoms.size(); i++)
    {
        file << all_atoms[i].x << " " << all_atoms[i].y << " " << all_atoms[i].z << std::endl;
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
        atom.x = std::stoi(words[0]);
        atom.y = std::stoi(words[1]);
        atom.z = std::stoi(words[2]);
        all_atoms.push_back(atom);


    }

    return all_atoms;

}

std::vector<std::string> split_sentence(std::string sentence) 
{
  
    // Create a stringstream object
    std::stringstream ss(sentence);
    
    // Variable to hold each word
    std::string word;
    
    // Vector to store the words
    std::vector<std::string> words;
    
    // Extract words from the sentence
    while (ss >> word) {
      
        // Add the word to the vector
        words.push_back(word);
    }
    
    return words;
}

#endif