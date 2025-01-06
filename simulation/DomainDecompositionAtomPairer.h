#pragma once
#include <vector>
#include "Atom.h"

class AtomPairer
{
    public:
        AtomPairer(std::vector<Atom> &atoms)
        {
            this->atoms = atoms;
        }

        std::vector<Atom> atoms;

        virtual void pair_atoms() = 0;
};

class DomainDecompositionAtomPairer : public AtomPairer
{
    public:
        DomainDecompositionAtomPairer(std::vector<Atom> &atoms) : AtomPairer(atoms)
        {

        }

        void pair_atoms()
        {

        }
};
