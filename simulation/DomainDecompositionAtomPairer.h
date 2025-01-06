#pragma once
#include <vector>
#include "Atom.h"
#include <tuple>
#include <vector>


class AtomPairer
{
    public:
        AtomPairer(std::vector<Atom> &atoms)
        {
            this->atoms = atoms;
        }

        std::vector<Atom> atoms;

        virtual std::vector<std::tuple<int, int>> pair_atoms() = 0;
};

class DomainDecompositionAtomPairer : public AtomPairer
{
    public:
        DomainDecompositionAtomPairer(std::vector<Atom> &atoms) : AtomPairer(atoms)
        {

        }

        std::vector<std::tuple<int, int>> pair_atoms()
        {

        }
};
