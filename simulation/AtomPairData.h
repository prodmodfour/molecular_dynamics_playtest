#pragma once
#include <string>


class AtomPairData
{
    public:
        AtomPairData(std::string atom_type_1, std::string atom_type_2, double epsilon, double sigma)
        {
            this->atom_type_1 = atom_type_1;
            this->atom_type_2 = atom_type_2;
            this->epsilon = epsilon;
            this->sigma = sigma;

        }

        std::string atom_type_1;
        std::string atom_type_2;
        double epsilon;
        double sigma;
};
