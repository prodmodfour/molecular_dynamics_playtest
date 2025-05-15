#include "AtomPairData.h"

namespace atoms {

AtomPairData::AtomPairData(const std::string atom_type_1,
                           const std::string atom_type_2,
                           double epsilon,
                           double sigma)
    : atom_type_1_(atom_type_1),
      atom_type_2_(atom_type_2),
      epsilon_(epsilon),
      sigma_(sigma) {}


const std::string AtomPairData::get_atom_type_1() { return atom_type_1_; }

const std::string AtomPairData::get_atom_type_2() { return atom_type_2_; }

double AtomPairData::get_epsilon() { return epsilon_; }

double AtomPairData::get_sigma() { return sigma_; }

}