#include <vector>
#include <vtkAnimationCue.h>
#include <vtkAnimationScene.h>
#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkLogger.h>
#include <vtkActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkVectorOperators.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkGlyph3DMapper.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkUnsignedCharArray.h>
#include "Settings.h"
#include "Atom.h"
#include "AtomGenerator.h"
#include "SimulationConfig.h"

#include "simulation.h"
#include "file_functions.h"
#include "SimulationData.h"
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <map>
#include <string>

int main() {
    SimulationConfig config;
    
    std::cout << "Setting up atoms" << std::endl << std::endl;
    std::vector<Atom> all_atoms;
    AtomGenerator atom_gen(config);

    if (settings.get_atom_mode() == "generate") {
        all_atoms = atom_gen.generate_fcc();
    }

    // Add an impact atom if required
    if (settings.get_add_impact_on()) {
        atom_gen.add_impact_atom(all_atoms, settings.get_impact_surface());
    }

    std::cout << "Atoms initialized: " << all_atoms.size() << std::endl;

    // Create the simulation data
    SimulationData simData(all_atoms, settings);

    // Create the visualizer
    MDVisualiser visualiser(simData);
    
    // Start a separate thread for molecular dynamics simulation
    std::thread simulationThread([&simData, &settings, &atom_gen]() {
        int timesteps_per_frame = 5;
        while (true) {
            if (simData.buffer_full()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            Frame frame = simData.get_latest_frame();

            performance_monitor.start_frame("Simulation");
            frame = create_next_frame(frame, settings, timesteps_per_frame);
            simData.add_frame(frame);
        }
    });

    // Launch the visualization
    visualiser.launch(simData);

    // Join the simulation thread after visualization ends
    simulationThread.join();

    return 0;
}
