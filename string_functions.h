#ifndef __string_functions_h
#define __string_functions_h

#include <vector>
#include <string>
#include <sstream>
#include <iostream>


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

bool flag_in_arguments_exactly_once(std::vector<std::string> arguments, std::string flag)
{
    int count = 0;
    for (int i = 0; i < arguments.size(); i++)
    {
        if (arguments[i] == flag)
        {
            count++;
        }
    }

    if (count == 1)
    {
        return true;
    }
    else if (count == 0)
    {
        return false;
    }
    else if (count > 1)
    {
        std::cout << "Flag " << flag << " used more than once (" << count << " times) and was ignored." << std::endl;
        return false;
    }

    return false;
}

#endif