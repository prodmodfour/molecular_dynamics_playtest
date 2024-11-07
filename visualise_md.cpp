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

#include "simulation.h"
#include "file_functions.h"
#include "SimulationHistory.h"
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
#include "MDMainWindow.h"
#include <QApplication>

bool pauseAnimation = false;  // Global pause flag for controlling animation
int playback_speed = 1;
const int frame_rate = 30;
int playback_direction = 1; // 1 for forwards, -1 for backward

class PerformanceMonitor {
private:
    struct ThreadStats {
        double avg_frame_time = 0.0;
        int frame_count = 0;
        std::chrono::high_resolution_clock::time_point last_report_time;
    };
    
    std::map<std::string, ThreadStats> stats;
    const double REPORT_INTERVAL = 5.0; // Report every 5 seconds
    std::mutex print_mutex;

public:
    void start_frame(const std::string& thread_name) {
        thread_local auto last_frame_start = std::chrono::high_resolution_clock::now();
        auto now = std::chrono::high_resolution_clock::now();
        
        auto& thread_stats = stats[thread_name];
        double frame_time = std::chrono::duration<double>(now - last_frame_start).count();
        
        // Update moving average
        thread_stats.avg_frame_time = (thread_stats.avg_frame_time * thread_stats.frame_count + frame_time) 
                                    / (thread_stats.frame_count + 1);
        thread_stats.frame_count++;
        
        // Report if interval elapsed
        if (std::chrono::duration<double>(now - thread_stats.last_report_time).count() > REPORT_INTERVAL) {
            std::lock_guard<std::mutex> lock(print_mutex);
            double fps = 1.0 / thread_stats.avg_frame_time;
            std::cout << thread_name << " Performance: " 
                     << "Avg Snapshot Time: " << thread_stats.avg_frame_time * 1000.0 << "ms, "
                     << "FPS: " << fps << std::endl;
            
            thread_stats.last_report_time = now;
            thread_stats.frame_count = 0;
            thread_stats.avg_frame_time = 0;
        }
        
        last_frame_start = now;
    }
};

PerformanceMonitor performance_monitor;

// Function to compute atom color based on kinetic energy
void get_atom_color(const Atom& atom, unsigned char color[3]) {
    double ratio = atom.ke / atom.reference_ke;
    if (ratio > 1)
    {
        ratio = 1;
    }
    else if (ratio < 0)
    {
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
    double min_opacity = 0.1;  // Minimum opacity 
    double opacity_ratio = min_opacity + ratio * (1.0 - min_opacity);
    color[3] = static_cast<unsigned char>(opacity_ratio * 255.0);  // Alpha component
}

// Timer callback for non-blocking animation
class TimerCallback : public vtkCommand {
public:
    static TimerCallback* New() {
        return new TimerCallback;
    }

    void Execute(vtkObject* caller, unsigned long eventId, void* callData) override {
        if (!pauseAnimation) {
            for (int i = 0; i < playback_speed; i++) {
                bool success;
                if (playback_direction > 0) {
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
                Snapshot frame = simData->get_current_frame();
                performance_monitor.start_frame("Animation");
                updateSceneWithFrame(frame);
            }
            renderWindow->Render();
        }
    }

    SimulationHistory* simData;
    vtkRenderWindow* renderWindow;
    vtkRenderer* renderer;
    vtkSmartPointer<vtkPolyData> polyData;
    vtkSmartPointer<vtkTextActor> reading_actor;

    // Function to update the scene
    void updateSceneWithFrame(const Snapshot& frame) {
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
};

// Function to initialize the polyData with atom positions and colors
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

// Function to handle keyboard events
class KeyPressCallback : public vtkCommand {
public:
    static KeyPressCallback* New() {
        return new KeyPressCallback;
    }

    virtual void Execute(vtkObject* caller, unsigned long eventId, void* callData) override {
        vtkRenderWindowInteractor* interactor = static_cast<vtkRenderWindowInteractor*>(caller);
        std::string key = interactor->GetKeySym();

        if (key == "space") {
            pauseAnimation = !pauseAnimation;
            if (pauseAnimation) {
                std::cout << "Animation Paused" << std::endl;
            } else {
                std::cout << "Animation Resumed" << std::endl;
            }
        } else if (key == "plus" || key == "equal") {
            adjustPlaybackSpeed(1);
        } else if (key == "minus" || key == "underscore") {
            adjustPlaybackSpeed(-1);
        } else if (key == "r") {
            playback_direction *= -1;
            std::cout << "Playback direction: " 
                      << (playback_direction > 0 ? "forward" : "backward") << std::endl;
        } else if (key == "b") {
            // Create new branch with modified settings
            Settings branch_settings = simData->get_current_settings();
            branch_settings.set_energy_applied_to_impact_atom(
                branch_settings.get_energy_applied_to_impact_atom() * 1.5
            );
            std::string new_branch_id = "branch_" + std::to_string(time(nullptr));
            simData->create_branch(new_branch_id, branch_settings);
            std::cout << "Created new branch: " << new_branch_id << std::endl;
        } else if (key == "n") {
            // Cycle through available branches
            auto branches = simData->get_available_branches();
            auto current = simData->get_active_branch_id();
            auto it = std::find(branches.begin(), branches.end(), current);
            if (it != branches.end() && ++it != branches.end()) {
                simData->switch_branch(*it);
                std::cout << "Switched to branch: " << *it << std::endl;
            } else if (!branches.empty()) {
                simData->switch_branch(branches.front());
                std::cout << "Switched to branch: " << branches.front() << std::endl;
            }
        }
    }

    SimulationHistory* simData;
private:
    void adjustPlaybackSpeed(int delta) {
        playback_speed = std::max(1, playback_speed + delta);
        std::cout << "Playback speed: " << playback_speed << "x" << std::endl;
    }
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    std::vector<std::string> arguments(argv, argv + argc);
    Settings settings(arguments);
    settings.print_all_settings();

    std::cout << "Setting up atoms" << std::endl << std::endl;
    std::vector<Atom> all_atoms;
    AtomGenerator atom_gen(settings);

    if (settings.get_atom_mode() == "generate") {
        all_atoms = atom_gen.generate_fcc();
    }

    // Add an impact atom if required
    if (settings.get_add_impact_on() && !settings.get_bombardment_on()) {
        atom_gen.add_impact_atom(all_atoms, settings.get_impact_surface());
    }

    std::cout << "Atoms initialized: " << all_atoms.size() << std::endl;

    // Create the simulation data
    SimulationHistory simData(all_atoms, settings);

        
    // Start Main window
    MDMainWindow window(&simData, &settings);
    window.show();

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
    keyPressCallback->simData = &simData; 
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
        double velocity_scale = settings.get_velocity_scale();
        std::map<std::string, double> next_bombardment_times;

        while (true) {
            if (simData.buffer_full()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            
            std::string current_branch_id = simData.get_active_branch_id();
            Settings& branch_settings = simData.get_branch_settings(current_branch_id);
            
            Snapshot frame = simData.get_latest_frame();

            // Initialize bombardment time for new branches
            if (branch_settings.get_bombardment_on() && 
                next_bombardment_times.find(current_branch_id) == next_bombardment_times.end()) {
                next_bombardment_times[current_branch_id] = frame.time + branch_settings.get_bombardment_interval();
            }

            // Handle bombardment with branch-specific settings
            if (branch_settings.get_bombardment_on()) {
                double& next_bombardment_time = next_bombardment_times[current_branch_id];
                
                if (frame.time >= next_bombardment_time) {
                    if (branch_settings.get_bombardment_mode() == "consistent") {
                        atom_gen.add_impact_atom(frame.all_atoms, branch_settings.get_impact_surface());
                    } 
                    else if (branch_settings.get_bombardment_mode() == "spread") {
                        atom_gen.add_random_impact_atom(frame.all_atoms, branch_settings.get_impact_surface());
                    } 
                    else if (branch_settings.get_bombardment_mode() == "3d_spread") {
                        atom_gen.add_impact_atom_random_surface(frame.all_atoms);
                    }

                    std::cout << "Added impact atom at time " << frame.time 
                            << " in branch " << current_branch_id << std::endl;
                    next_bombardment_time = frame.time + branch_settings.get_bombardment_interval();
                }
            }

            performance_monitor.start_frame("Simulation");
            
            // Use branch-specific settings for simulation
            frame = create_next_frame(frame, branch_settings, timesteps_per_frame);
            frame.branch_id = current_branch_id;  // Ensure frame knows its branch
            
            simData.add_frame(frame);
        }
    });

    // Start the interaction loop
    renderWindow->Render();
    renderWindowInteractor->Start();

    int result = app.exec();

    // Join the simulation thread after animation ends
    simulationThread.join();

    return result;
}
