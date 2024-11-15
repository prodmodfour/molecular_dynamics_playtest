#include "Launcher.h"
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QThread>
#include "SimulationConfig.h"
#include "Atom.h"
#include "AtomGenerator.h"
#include "SimulationData.h"
#include "MDVisualiser.h"

Launcher::Launcher(QWidget* parent)
    : QMainWindow(parent) {

    // Central widget and layout
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QFormLayout* formLayout = new QFormLayout(centralWidget);

    // Atom mass input
    atomMassSpinBox = new QDoubleSpinBox();
    atomMassSpinBox->setRange(0.1, 1000.0);
    atomMassSpinBox->setValue(63.546);
    formLayout->addRow("Atom Mass", atomMassSpinBox);

    // Cubes in X
    cubesInXSpinBox = new QSpinBox();
    cubesInXSpinBox->setRange(1, 100);
    cubesInXSpinBox->setValue(8);
    formLayout->addRow("Cubes in X", cubesInXSpinBox);

    // Cubes in Y
    cubesInYSpinBox = new QSpinBox();
    cubesInYSpinBox->setRange(1, 100);
    cubesInYSpinBox->setValue(4);
    formLayout->addRow("Cubes in Y", cubesInYSpinBox);

    // Cubes in Z
    cubesInZSpinBox = new QSpinBox();
    cubesInZSpinBox->setRange(1, 100);
    cubesInZSpinBox->setValue(4);
    formLayout->addRow("Cubes in Z", cubesInZSpinBox);

    // Atom spacing
    atomSpacingSpinBox = new QDoubleSpinBox();
    atomSpacingSpinBox->setRange(0.1, 10.0);
    atomSpacingSpinBox->setValue(3.61);
    formLayout->addRow("Atom Spacing", atomSpacingSpinBox);

    // Energy applied
    energyAppliedSpinBox = new QDoubleSpinBox();
    energyAppliedSpinBox->setRange(0.0, 1000.0);
    energyAppliedSpinBox->setValue(10);
    formLayout->addRow("Energy Applied to Impact Atom", energyAppliedSpinBox);

    // Add impact atom
    addImpactCheckBox = new QCheckBox();
    addImpactCheckBox->setChecked(true);
    formLayout->addRow("Add Impact Atom", addImpactCheckBox);

    // Start button
    startButton = new QPushButton("Start Simulation");
    formLayout->addWidget(startButton);

    // Connect the start button
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startSimulation);
}

void MainWindow::startSimulation() {
    // Gather input data
    SimulationConfig config;
    config.setAtomMass(atomMassSpinBox->value());
    config.cubes_in_x = cubesInXSpinBox->value();
    config.cubes_in_y = cubesInYSpinBox->value();
    config.cubes_in_z = cubesInZSpinBox->value();
    config.atom_spacing = atomSpacingSpinBox->value();
    config.energy_applied_to_impact_atom = energyAppliedSpinBox->value();
    config.add_impact_on = addImpactCheckBox->isChecked();

    
    // Generate atoms
    AtomGenerator atom_gen(config);
    std::vector<Atom> all_atoms = atom_gen.generate_fcc();

    // Add impact atom if required
    if (config.add_impact_on) {
        atom_gen.add_impact_atom(all_atoms, "top");
    }



    // Create simulation data
    SimulationData* simData = new SimulationData(all_atoms, config);

    // Create and show the visualizer
    MDVisualiser* visualiser = new MDVisualiser(*simData);
    visualiser->launch();  // This shows the visualizer window

    // Start the simulation thread
    std::thread* simulationThread = new std::thread([simData, config]() {
        int timesteps_per_frame = 5;
        while (true) {
            if (simData->buffer_full()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            Frame frame = simData->get_latest_frame();

            // Compute the next frame
            frame = create_next_frame(frame, config, timesteps_per_frame);
            simData->add_frame(frame);
        }
    });

    // Cleanup when the visualizer window is closed
    connect(visualiser, &QMainWindow::destroyed, [simulationThread, simData]() {
        // Terminate the simulation thread

        simulationThread->detach();

    });
}