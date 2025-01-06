#pragma once
#include "AtomPairLibrary.h"
#include "Atom.h"
#include <vector>

class ForceEvaluator
{
    public:
        ForceEvaluator(AtomPairLibrary &atom_pair_library, std::vector<Atom> &atoms)
        {
            this->atom_pair_library = atom_pair_library;
            this->atoms = atoms;
        }

        AtomPairLibrary atom_pair_library;
        std::vector<Atom> atoms;
        

};