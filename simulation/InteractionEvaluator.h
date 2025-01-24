#pragma once
#include "AtomPairLibrary.h"
#include "Atom.h"
#include <vector>
#include "Config.h"
#include "ForceEvaluator.h"
#include "PotentialEnergyCalculator.h"
#include "EnergyTracker.h"

class InteractionEvaluator
{
    public:
        InteractionEvaluator(AtomPairLibrary &atom_pair_library, std::vector<Atom> &atoms)
        {
            this->atom_pair_library = atom_pair_library;
            this->atoms = atoms;
            this->force_evaluator = ForceEvaluator(atom_pair_library, atoms);
            this->potential_energy_calculator = PotentialEnergyCalculator(atom_pair_library, atoms);
        }

        AtomPairLibrary atom_pair_library;
        std::vector<Atom> atoms;
        ForceEvaluator force_evaluator;
        PotentialEnergyCalculator potential_energy_calculator;

        void evaluate_interactions(Config config, TotalEnergy &total_energy)
        {
            force_evaluator.zero_forces();

            double xi, yi, zi;
            double xj, yj, zj;
            double dx, dy, dz;
            double fxij, fyij, fzij, fij;
            double squared_distance, distance, sigma_squared;
            double sr2, sr6, sr12, epsilon4, epsilon24;
            double potential_ij, potential_energy, unit_vector_x, unit_vector_y, unit_vector_z;
            double reciprocal_distance;

        }
}
