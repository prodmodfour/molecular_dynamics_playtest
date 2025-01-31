#pragma once
#include "Config.h"
#include "Atom.h"
#include <vector>
#include "AtomPairLibrary.h"
#include "InteractionEvaluator.h"
#include "MotionCalculator.h"
#include "Timestep.h"
#include "TotalEnergy.h"


class MDSimulator
{
    public:
        MDSimulator(AtomPairLibrary &atom_pair_library, double &time)
        {
            this->atom_pair_library = atom_pair_library;
            this->interaction_evaluator = InteractionEvaluator(atom_pair_library, atoms);
            this->motion_calculator = MotionCalculator(atoms);
            this->time = time;
        }

        AtomPairLibrary atom_pair_library;
        InteractionEvaluator interaction_evaluator;
        MotionCalculator motion_calculator;

        double time;

        Timestep simulate_timestep(Config config, std::vector<Atom> atoms)
        {
            TotalEnergy total_energy;   
            interaction_evaluator.evaluate_interactions(config, total_energy);

            for (Atom &atom : atoms)
            {
                motion_calculator.calculate_motion(config, atom);
            }

            double total_kinetic_energy = 0;
            for (Atom &atom : atoms)
            {
                total_kinetic_energy += atom.kinetic_energy;
            }

            total_energy.kinetic = total_kinetic_energy;
            time += config.timestep_size;

            return Timestep(config, atoms, total_energy);
        }
};
