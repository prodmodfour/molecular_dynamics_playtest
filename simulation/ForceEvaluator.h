#pragma once
#include "AtomPairLibrary.h"
#include "Atom.h"
#include <vector>
#include "AtomPairer.h"
#include <tuple>
#include "AtomPairData.h"
#include "Config.h"
#include <cmath>

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


        void evaluate_forces(Config config, std::vector<std::tuple<int, int, double, double>> atom_pair)
        {
            
            double r_cutoff = config.r_cutoff;
            double r_cutoff_squared = r_cutoff * r_cutoff;

            Atom atom1 = atoms[std::get<0>(atom_pair)];
            Atom atom2 = atoms[std::get<1>(atom_pair)];
            double r = std::get<2>(atom_pair);
            double r_squared = r * r;
            AtomPairData atom_pair_data = atom_pair_library.get_atom_pair_data(atom1.type, atom2.type);
            double epsilon = atom_pair_data.epsilon;
            double sigma = atom_pair_data.sigma;
            double sigma_squared = sigma * sigma;

            double sr2 = sigma_squared / r_squared;
            double sr6 = sr2 * sr2 * sr2;
            double sr12 = sr6 * sr6;

            double reciprocal_r = 1.0 / r;


        }

        void zero_forces()
        {
            for (Atom &atom : atoms)
            {
                atom.fx = 0.0;
                atom.fy = 0.0;
                atom.fz = 0.0;
            }
        }
};