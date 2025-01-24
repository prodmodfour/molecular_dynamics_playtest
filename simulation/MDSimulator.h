#pragma once
#include "Config.h"
#include "Atom.h"
#include <vector>
#include "AtomPairer.h"
#include "AtomPairLibrary.h"
#include "ForceEvaluator.h"
#include "PotentialEnergyCalculator.h"
#include "InteractionEvaluator.h"
#include "MotionCalculator.h"
#include "EnergyTracker.h"



class MDSimulator
{
    public:
        MDSimulator(std::vector<Atom> &atoms, AtomPairLibrary &atom_pair_library, double &time)
        {
            this->atoms = atoms;
            this->atom_pair_library = atom_pair_library;
            this->interaction_evaluator = InteractionEvaluator(atom_pair_library, atoms);
            this->motion_calculator = MotionCalculator(atoms);
            this->energy_tracker = EnergyTracker();
            this->time = time;
        }

        std::vector<Atom> atoms;
        AtomPairLibrary atom_pair_library;
        InteractionEvaluator interaction_evaluator;
        MotionCalculator motion_calculator;
        EnergyTracker energy_tracker;
        double time;

        void simulate_timestep(Config config)
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
            energy_tracker.add_total_energy_to_history(total_energy);

            time += config.timestep_size;
        }
};
