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
    if (settings.get_add_impact_on() && !settings.get_bombardment_on()) {
        atom_gen.add_impact_atom(all_atoms, settings.get_impact_surface());
    }

    std::cout << "Atoms initialized: " << all_atoms.size() << std::endl;

    // Create the simulation data
    SimulationData simData(all_atoms, settings);

    // Create the VTK components for rendering
    vtkNew<vtkNamedColors> colors;
    vtkNew<vtkRenderer> renderer;
    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    renderWindow->SetWindowName("Molecular Dynamics Simulation");
    renderWindow->SetSize(1280, 720);
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    renderWindowInteractor->SetRenderWindow(renderWindow);

    // Initialize polyData and glyph mapper
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();

    initialise_polydata(all_atoms, polyData);
    

    // Create sphere source
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    double atom_radius = settings.get_atom_radius();

    std::cout << "Atom radius: " << atom_radius << std::endl;
    sphereSource->SetRadius(atom_radius);

    // Set up glyph mapper
    vtkSmartPointer<vtkGlyph3DMapper> glyphMapper = vtkSmartPointer<vtkGlyph3DMapper>::New();
    glyphMapper->SetInputData(polyData);
    glyphMapper->SetSourceConnection(sphereSource->GetOutputPort());
    glyphMapper->ScalarVisibilityOn();
    glyphMapper->SetScalarModeToUsePointFieldData();
    glyphMapper->SelectColorArray("Colors");
    glyphMapper->SetColorModeToDirectScalars();
    glyphMapper->SetUseLookupTableScalarRange(false);

    // Create actor and add to renderer
    vtkSmartPointer<vtkActor> glyphActor = vtkSmartPointer<vtkActor>::New();
    glyphActor->SetMapper(glyphMapper);
    // Enable per-vertex opacity
    glyphActor->GetProperty()->SetOpacity(1.0);
    renderer->AddActor(glyphActor);

    // Create text actor for displaying simulation information
    vtkSmartPointer<vtkTextActor> reading_actor = vtkSmartPointer<vtkTextActor>::New();
    reading_actor->SetInput("Time: 0.000 s TE: 0.0 KE: 0.0 PE 0.0");
    reading_actor->GetTextProperty()->SetFontSize(24);
    reading_actor->GetTextProperty()->SetColor(1.0, 1.0, 1.0);
    reading_actor->SetPosition(10, 10);
    renderer->AddActor2D(reading_actor);

    // Set up the timer-based callback for non-blocking animation
    vtkNew<TimerCallback> timerCallback;
    timerCallback->simData = &simData;
    timerCallback->renderWindow = renderWindow;
    timerCallback->renderer = renderer;
    timerCallback->polyData = polyData;
    timerCallback->reading_actor = reading_actor;

    vtkNew<vtkInteractorStyleTrackballCamera> style;
    renderWindowInteractor->SetInteractorStyle(style);

    renderWindowInteractor->Initialize();

    renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, timerCallback);
    const int timer_interval = 1000 / frame_rate;
    renderWindowInteractor->CreateRepeatingTimer(timer_interval);

    // Set up the keypress callback
    vtkNew<KeyPressCallback> keyPressCallback;
    renderWindowInteractor->AddObserver(vtkCommand::KeyPressEvent, keyPressCallback);

    if (settings.get_parallel_projection_on()) {
        renderer->GetActiveCamera()->ParallelProjectionOn();
    }
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Dolly(0.5);
    renderer->ResetCameraClippingRange();

    // Enable alpha channel and disable multisampling
    renderWindow->SetAlphaBitPlanes(1);
    renderWindow->SetMultiSamples(0);

    // Configure renderer for depth peeling
    renderer->SetUseDepthPeeling(1);
    renderer->SetMaximumNumberOfPeels(100);
    renderer->SetOcclusionRatio(0.1);

    // Start a separate thread for molecular dynamics simulation
    std::thread simulationThread([&simData, &settings, &atom_gen]() {
        int timesteps_per_frame = 5;
        double next_bombardment_time = settings.get_bombardment_interval();
        while (true) {
            if (simData.buffer_full()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            Frame frame = simData.get_latest_frame();

            if (settings.get_bombardment_on()) {
                if (frame.time >= next_bombardment_time) {
                    if (settings.get_bombardment_mode() == "consistent") {
                        atom_gen.add_impact_atom(frame.all_atoms, settings.get_impact_surface());
                    } else if (settings.get_bombardment_mode() == "spread") {
                        atom_gen.add_random_impact_atom(frame.all_atoms, settings.get_impact_surface());
                    } else if (settings.get_bombardment_mode() == "3d_spread") {
                        atom_gen.add_impact_atom_random_surface(frame.all_atoms);
                    }

                    std::cout << "Added impact atom at " << frame.time << std::endl;
                    next_bombardment_time += settings.get_bombardment_interval();
                }
            }
            performance_monitor.start_frame("Simulation");
            frame = create_next_frame(frame, settings, timesteps_per_frame);
            simData.add_frame(frame);
        }
    });

    // Start the interaction loop
    renderWindow->Render();
    renderWindowInteractor->Start();

    // Join the simulation thread after animation ends
    simulationThread.join();

    return 0;
}
