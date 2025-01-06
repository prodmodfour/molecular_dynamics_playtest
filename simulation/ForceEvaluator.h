#pragma once
#include "AtomPairLibrary.h"
#include "Atom.h"
#include <vector>
#include "DomainDecompositionAtomPairer.h"
#include <tuple>
#include "AtomPairData.h"

class ForceEvaluator
{
    public:
        ForceEvaluator(AtomPairLibrary &atom_pair_library, std::vector<Atom> &atoms, AtomPairer *atom_pairer)
        {
            this->atom_pair_library = atom_pair_library;
            this->atoms = atoms;
            this->atom_pairer = atom_pairer;
        }

        AtomPairLibrary atom_pair_library;
        std::vector<Atom> atoms;
        AtomPairer *atom_pairer;

        void evaluate_forces()
        {
            std::vector<std::tuple<int, int>> atom_pairs = atom_pairer->pair_atoms();
            for (std::tuple<int, int> atom_pair : atom_pairs)
            {
                Atom atom1 = atoms[std::get<0>(atom_pair)];
                Atom atom2 = atoms[std::get<1>(atom_pair)];
                AtomPairData atom_pair_data = atom_pair_library.get_atom_pair_data(atom1.type, atom2.type);
            }
        }
};