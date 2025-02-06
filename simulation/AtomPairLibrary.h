#pragma once
#include <string>
#include <vector>
#include "AtomPairData.h"
#include <stdexcept>


class AtomPairLibrary
{
    public:
        AtomPairLibrary()
        {
            atom_pair_data = std::vector<AtomPairData>();
            atom_pair_data.push_back(AtomPairData("Cu", "Cu", 1.0, 1.0));
        }
        std::vector<AtomPairData> atom_pair_data;

        AtomPairData get_atom_pair_data(std::string atom_type_1, std::string atom_type_2)
        {
            for (AtomPairData &atom_pair_data : atom_pair_data)
            {
                if (atom_pair_data.atom_type_1 == atom_type_1 && atom_pair_data.atom_type_2 == atom_type_2)
                {
                    return atom_pair_data;
                }
                else if (atom_pair_data.atom_type_1 == atom_type_2 && atom_pair_data.atom_type_2 == atom_type_1)
                {
                    return atom_pair_data;
                }
                else
                {
                    throw std::runtime_error("Atom pair data not found");
                }
            }
        }

        void add_atom_pair_data(AtomPairData atom_pair_data)
        {
            this->atom_pair_data.push_back(atom_pair_data);
        }
};