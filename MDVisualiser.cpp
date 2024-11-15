#include "MDVisualiser.h"
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPointData.h>
#include <vtkUnsignedCharArray.h>
#include <iomanip>
#include <sstream>
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

std::mutex timeline_mutex;

void get_atom_color(const Atom& atom, unsigned char color[3]) {
    double ratio = atom.ke / atom.reference_ke;
    if (ratio > 1) {
        ratio = 1;
    }
    else if (ratio < 0) {
        ratio = 0;
    }

    double red = ratio * 255.0;
    double blue = (1.0 - ratio) * 255.0;

    color[0] = static_cast<unsigned char>(red);
    color[1] = 0; 
    color[2] = static_cast<unsigned char>(blue);

    // Calculate opacity based on redness
    // Hotter atoms (redder) are more opaque
    // Cooler atoms (bluer) are less opaque but have a minimum opacity
    double min_opacity = 0.1;  // Minimum opacity (20% opaque)
    double opacity_ratio = min_opacity + ratio * (1.0 - min_opacity);
    color[3] = static_cast<unsigned char>(opacity_ratio * 255.0);  // Alpha component
}

void initialise_polydata(const std::vector<Atom>& all_atoms, vtkSmartPointer<vtkPolyData>& polyData) {
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(4);  // RGBA colors
    colors->SetName("Colors");

    for (const Atom& atom : all_atoms) {
        points->InsertNextPoint(atom.x, atom.y, atom.z);

        unsigned char color[4];
        get_atom_color(atom, color);
        colors->InsertNextTypedTuple(color); 
    }

    polyData->SetPoints(points);
    polyData->GetPointData()->AddArray(colors);
}

TimerCallback* TimerCallback::New() {
    return new TimerCallback;
}

void TimerCallback::Execute(vtkObject* caller, unsigned long eventId, void* callData) {
    if (!visualiser->pauseAnimation) {
        for (int i = 0; i < visualiser->playback_speed; i++) {
            bool success;
            if (visualiser->playback_direction > 0) {
                success = simData->move_forward();
                if (!success) {
                    // Wait for new frames or pause if simulation has ended
                    break;
                }
            } else {
                success = simData->move_backward();
                if (!success) {
                    break;
                }
            }
            Frame frame = simData->get_current_frame();
            updateSceneWithFrame(frame);
        }
        renderWindow->Render();
    }
}

void TimerCallback::updateSceneWithFrame(const Frame& frame) {
    vtkPoints* points = polyData->GetPoints();
    vtkUnsignedCharArray* colors = vtkUnsignedCharArray::SafeDownCast(polyData->GetPointData()->GetArray("Colors"));

    size_t numAtoms = frame.all_atoms.size();

    // Resize points and colors if number of atoms has changed
    if (points->GetNumberOfPoints() != numAtoms) {
        points->SetNumberOfPoints(numAtoms);
        colors->SetNumberOfTuples(numAtoms);
    }

    for (vtkIdType i = 0; i < numAtoms; ++i) {
        const Atom& atom = frame.all_atoms[i];
        points->SetPoint(i, atom.x, atom.y, atom.z);

        unsigned char color[4];
        get_atom_color(atom, color);
        colors->SetTypedTuple(i, color); 
    }

    points->Modified();
    colors->Modified();

    // Update text actor with simulation data
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3) << frame.time;
    std::string time_string = oss.str();

    std::ostringstream oss1;
    oss1 << std::setprecision(4) << frame.te;
    std::string te_string = oss1.str();
    oss1.str("");
    oss1.clear();
    oss1 << std::setprecision(4) << frame.ke;
    std::string ke_string = oss1.str();
    oss1.str("");
    oss1.clear();
    oss1 << std::setprecision(4) << frame.pe;
    std::string pe_string = oss1.str();
    oss1.str("");
    oss1.clear();
    double average_ke = frame.ke / frame.all_atoms.size();
    oss1 << std::setprecision(4) << average_ke;
    std::string ake_string = oss1.str();

    std::string reading = "Time: " + time_string + " ps " +
                          " TE: " + te_string + " eV " +
                          " KE: " + ke_string + " eV " +
                          " PE: " + pe_string + " eV" +
                          " Average KE: " + ake_string + " eV";
    reading_actor->SetInput(reading.c_str());
}

KeyPressCallback* KeyPressCallback::New() {
    return new KeyPressCallback;
}

void KeyPressCallback::Execute(vtkObject* caller, unsigned long eventId, void* callData) {
    vtkRenderWindowInteractor* interactor = static_cast<vtkRenderWindowInteractor*>(caller);
    std::string key = interactor->GetKeySym();

    if (key == "space") {
        visualiser->pauseAnimation = !visualiser->pauseAnimation;
        if (visualiser->pauseAnimation) {
            std::cout << "Animation Paused" << std::endl;
        } else {
            std::cout << "Animation Resumed" << std::endl;
        }
    } else if (key == "plus" || key == "equal") {
        adjustPlaybackSpeed(1);
    } else if (key == "minus" || key == "underscore") {
        adjustPlaybackSpeed(-1);
    } else if (key == "r") {
        visualiser->playback_direction *= -1;
        if (visualiser->playback_direction > 0) {
            std::cout << "Playback direction: forward" << std::endl;
        } else {
            std::cout << "Playback direction: backward" << std::endl;
        }
    }
}

void KeyPressCallback::adjustPlaybackSpeed(int delta) {
    visualiser->playback_speed = std::max(1, visualiser->playback_speed + delta);
}

MDVisualiser::MDVisualiser(SimulationData& sim_data) 
    : simData(sim_data), 
      all_atoms(sim_data.timeline[0].all_atoms),
      pauseAnimation(false), 
      playback_speed(1), 
      playback_direction(1),
      frame_rate(24) {
    std::cout << "Initializing visualizer with " << all_atoms.size() << " atoms" << std::endl;
    
    // Print first atom position for verification
    if (!all_atoms.empty()) {
        std::cout << "First atom position: (" 
                  << all_atoms[0].x << ", "
                  << all_atoms[0].y << ", " 
                  << all_atoms[0].z << ")" << std::endl;
    }
    
    polyData = vtkSmartPointer<vtkPolyData>::New();
    initialise_polydata(all_atoms, polyData);
}

void MDVisualiser::launch(SimulationData& simData) {
    // Create the renderer, render window, and interactor
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    renderWindow->SetWindowName("Molecular Dynamics Simulation");
    renderWindow->SetSize(1280, 720);
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renderWindow);

 // Create sphere source
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    double atom_radius = simData.config.atom_radius;

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

    // Create and position text actor for readings
    vtkSmartPointer<vtkTextActor> reading_actor = vtkSmartPointer<vtkTextActor>::New();
    reading_actor->SetPosition(10, 10);
    reading_actor->GetTextProperty()->SetFontSize(16);
    reading_actor->GetTextProperty()->SetColor(1.0, 1.0, 1.0);
    renderer->AddActor(reading_actor);

    // Set up timer callback
    vtkSmartPointer<TimerCallback> timerCallback = vtkSmartPointer<TimerCallback>::New();
    timerCallback->visualiser = this;
    timerCallback->simData = &simData;
    timerCallback->renderWindow = renderWindow;
    timerCallback->renderer = renderer;
    timerCallback->polyData = polyData;
    timerCallback->reading_actor = reading_actor;

    // Set up keyboard callback
    vtkSmartPointer<KeyPressCallback> keyCallback = vtkSmartPointer<KeyPressCallback>::New();
    keyCallback->visualiser = this;
    interactor->AddObserver(vtkCommand::KeyPressEvent, keyCallback);

    // Set up the animation timer
    interactor->Initialize();
    int frame_interval = static_cast<int>(1000.0 / frame_rate);  // Convert fps to milliseconds
    int timerId = interactor->CreateRepeatingTimer(frame_interval);
    interactor->AddObserver(vtkCommand::TimerEvent, timerCallback);
    
    // Add this debug output
    std::cout << "Number of actors in renderer: " << renderer->GetActors()->GetNumberOfItems() << std::endl;

    // Set background color and start the interaction
    renderer->SetBackground(0.0, 0.0, 0.0);
    renderWindow->Render();
    interactor->Start();


} 