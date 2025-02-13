#pragma once
#include "Config.h"
#include "Atom.h"
#include <vector>
#include "AtomPairLibrary.h"
#include "AtomPairData.h"
#include "InteractionEvaluator.h"
#include "MotionCalculator.h"
#include "Timestep.h"
#include "TotalEnergy.h"

namespace simulation {
class MDSimulator
{
    public:       
        AtomPairLibrary atom_pair_library;
        InteractionEvaluator interaction_evaluator;
        MotionCalculator motion_calculator;
        double time;

        MDSimulator(AtomPairLibrary &atom_pair_library, double &time)
            : atom_pair_library(atom_pair_library),
              interaction_evaluator(atom_pair_library),
              motion_calculator(),
              time(time)
        {
        }

        MDSimulator()
            : atom_pair_library(),
              interaction_evaluator(atom_pair_library),
              motion_calculator(),
              time(0)
        {
        }

        MDSimulator(double &time)
            : atom_pair_library(),
              interaction_evaluator(atom_pair_library),
              motion_calculator(),
              time(time)
        {
        }

        Timestep simulate_timestep(Config config, std::vector<Atom> atoms)
        {
            TotalEnergy total_energy(0, 0);   
            interaction_evaluator.evaluate_interactions(config, total_energy, atoms);

            for (Atom &atom : atoms)
            {
                motion_calculator.calculate_motion(config.timestep_size, atom);
            }

            double total_kinetic_energy = 0;
            for (Atom &atom : atoms)
            {
                total_kinetic_energy += atom.kinetic_energy;
            }

            total_energy.kinetic = total_kinetic_energy;
            time += config.timestep_size;

            return Timestep(config, atoms, total_energy, time);
        }
};

}