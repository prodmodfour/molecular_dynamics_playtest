#include <QApplication>
#include "InitialiserDialog.h"
#include "InitialConfig.h"
#include "Atom.h"
#include "FCCGenerator.h"
#include "Box.h"
#include "UnitVectorCalculator.h"
#include "KineticEnergyApplier.h"
#include "Vector.h"
#include "MDSimulator.h"
#include "Timestep.h"
#include <thread>
#include <iostream>


int main(int argc, char *argv[])
{
    // Launch Initialiser dialog box
     QApplication app(argc, argv);

    InitialiserDialog dialog;


    // Get Initial Config from initialiser dialog box
    if (dialog.exec() == QDialog::Accepted)
    {
        InitialConfig initial_config = dialog.getInitialConfig();
    }

    // Create atom structure
    FCCGenerator fcc_generator = FCCGenerator();
    std::vector<Atom> structure = fcc_generator.generate_fcc(initial_config.cubes_in_x, initial_config.cubes_in_y, initial_config.cubes_in_z, 3.61, "Cu", 63.546, 1.28);
    Box box = Box(structure);

    

    // Create impact atom
    Atom impact_atom = Atom("Cu", 63.546, 1.28);
    impact_atom.x = box.center.x + initial_config.impact_atom_offset.x;
    impact_atom.y = box.center.y + initial_config.impact_atom_offset.y;
    impact_atom.z = box.center.z + initial_config.impact_atom_offset.z;
    Vector impact_atom_position = Vector(impact_atom.x, impact_atom.y, impact_atom.z);

    // Apply kinetic energy to impact atom
    UnitVectorCalculator unit_vector_calculator = UnitVectorCalculator();
    Vector unit_vector = unit_vector_calculator.calculate_unit_vector_between_two_points(box.center, impact_atom_position);
    KineticEnergyApplier kinetic_energy_applier = KineticEnergyApplier();
    kinetic_energy_applier.apply_kinetic_energy(impact_atom, initial_config.impact_energy, unit_vector);

    // Initialise and add atoms to atom vector
    std::vector<Atom> atoms = structure.copy();
    atoms.push_back(impact_atom);

    std::vector<Timestep> simulation_data;
    Timestep initial_timestep = Timestep(initial_config.config, atoms, TotalEnergy(0, 0));
    simulation_data.push_back(initial_timestep);


    double time = 0;
    AtomPairLibrary atom_pair_library = AtomPairLibrary();

    // Launch simulator in its own thread
    std::thread simulator_thread([&]() {
        MDSimulator simulator = MDSimulator(atom_pair_library, time);
        for (int i = 0; i < 10000; i++)
        {
            Timestep timestep = simulator.simulate_timestep(initial_config.config, simulation_data.back().atoms);
            simulation_data.push_back(timestep);
        }
        std::cout << "Simulation complete" << std::endl;
        std::terminate();
    });
}



// Create atom structure

// Create impact atom

// Initialise and add atoms to atom vector

// Launch simulator in its own thread

// Launch visualiser in its own thread


