#pragma once
#include <string>

namespace atoms {


class AtomPairData {
public:

    AtomPairData(const std::string atom_type_1,
                 const std::string atom_type_2,
                 double epsilon,
                 double sigma);


    const std::string get_atom_type_1();

    const std::string get_atom_type_2();


    double get_epsilon();


    double get_sigma();

private:
    std::string atom_type_1_; 
    std::string atom_type_2_; 
    double      epsilon_;     
    double      sigma_;      
};



}