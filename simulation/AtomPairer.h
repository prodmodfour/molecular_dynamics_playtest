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

        virtual std::vector<std::tuple<int, int, double>> pair_atoms() = 0;
};

class DomainDecompositionAtomPairer : public AtomPairer
{
    public:
        DomainDecompositionAtomPairer(std::vector<Atom> &atoms) : AtomPairer(atoms)
        {

        }

        std::vector<std::tuple<int, int, double>> pair_atoms()
        {
            std::vector<std::tuple<int, int, double>> atom_pairs;

            return atom_pairs;

        }
};

class AlltoAllAtomPairer : public AtomPairer
{
    public:
        AlltoAllAtomPairer(std::vector<Atom> &atoms) : AtomPairer(atoms)
        {

        }

        std::vector<std::tuple<int, int, double>> pair_atoms(Config config)
        {
            std::vector<std::tuple<int, int, double>> atom_pairs;

            double r_cutoff = config.r_cutoff;
            double r_cutoff_squared = r_cutoff * r_cutoff;
            for (int i = 0; i < atoms.size(); i++)
            {
                for (int j = 0; j < atoms.size(); j++)
                {
                    atom_pairs.push_back(std::make_tuple(i, j, atoms[i].distance(atoms[j])));
                }
            }

            return atom_pairs;
        }
};

