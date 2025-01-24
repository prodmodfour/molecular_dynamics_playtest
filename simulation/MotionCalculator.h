#pragma once
#include "Atom.h"
#include <vector>
#include "PositionUpdater.h"
#include "VelocityUpdater.h"
#include "KineticEnergyCalculator.h"


class MotionCalculator
{
    public:
        MotionCalculator(std::vector<Atom> &atoms)
        {
            this->atoms = atoms;
            this->position_updater = PositionUpdater(atoms);
            this->velocity_updater = VelocityUpdater(atoms);
            this->kinetic_energy_calculator = KineticEnergyCalculator(atoms);
        }

        std::vector<Atom> atoms;
        PositionUpdater position_updater;
        VelocityUpdater velocity_updater;
        KineticEnergyCalculator kinetic_energy_calculator;

        void calculate_motion(Config config, Atom &atom)
        {
            position_updater.update_position(config, atom);
            velocity_updater.update_velocity(config, atom);
            kinetic_energy_calculator.calculate_kinetic_energy(config, atom);
        }
}
