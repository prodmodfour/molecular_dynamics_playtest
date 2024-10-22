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
#include "Settings.h"
#include "Atom.h"
#include "AtomGenerator.h"
#include "simulation.h"
#include "file_functions.h"
#include "SimulationData.h"
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <cmath>

bool pauseAnimation = false;
int playback_speed = 1;
const int frame_rate = 30;
int playback_direction = 1;

class PerformanceMonitor {
private:
    struct ThreadStats {
        double avg_frame_time = 0.0;
        int frame_count = 0;
        std::chrono::high_resolution_clock::time_point last_report_time;
    };
    std::map<std::string, ThreadStats> stats;
    const double REPORT_INTERVAL = 5.0;
    std::mutex print_mutex;

public:
    void start_frame(const std::string& thread_name) {
        thread_local auto last_frame_start = std::chrono::high_resolution_clock::now();
        auto now = std::chrono::high_resolution_clock::now();
        auto& thread_stats = stats[thread_name];
        double frame_time = std::chrono::duration<double>(now - last_frame_start).count();
        thread_stats.avg_frame_time = (thread_stats.avg_frame_time * thread_stats.frame_count + frame_time) 
                                    / (thread_stats.frame_count + 1);
        thread_stats.frame_count++;
        if (std::chrono::duration<double>(now - thread_stats.last_report_time).count() > REPORT_INTERVAL) {
            std::lock_guard<std::mutex> lock(print_mutex);
            double fps = 1.0 / thread_stats.avg_frame_time;
            std::cout << thread_name << " Performance: " 
                     << "Avg Frame Time: " << thread_stats.avg_frame_time * 1000.0 << "ms, "
                     << "FPS: " << fps << std::endl;
            thread_stats.last_report_time = now;
            thread_stats.frame_count = 0;
            thread_stats.avg_frame_time = 0;
        }
        last_frame_start = now;
    }
};

PerformanceMonitor performance_monitor;

void set_particle_color(vtkSmartPointer<vtkActor> actor, const Atom& atom) {
    double ratio = atom.ke / atom.reference_ke;
    if (ratio > 1) ratio = 1;
    else if (ratio < 0) ratio = 0;
    actor->GetProperty()->SetColor(ratio, 0.0, 1.0 - ratio);
}

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
                    if (!success) break;
                } else {
                    success = simData->move_backward();
                    if (!success) break;
                }
                Frame frame = simData->get_current_frame();
                performance_monitor.start_frame("Animation");
                updateSceneWithFrame(frame);
            }
            renderWindow->Render();
        }
    }

    SimulationData* simData;
    vtkRenderWindow* renderWindow;
    std::vector<vtkSmartPointer<vtkActor>>* atomActors;
    vtkSmartPointer<vtkTextActor> reading_actor;
    vtkRenderer* renderer;
    double atom_radius;

    void updateSceneWithFrame(const Frame& frame) {
        size_t numAtoms = frame.all_atoms.size();
        size_t numActors = atomActors->size();

        if (numAtoms > numActors) {
            for (size_t i = numActors; i < numAtoms; ++i) {
                vtkNew<vtkSphereSource> sphereSource;
                sphereSource->SetRadius(atom_radius);
                sphereSource->SetPhiResolution(8);
                sphereSource->SetThetaResolution(8);
                vtkNew<vtkPolyDataMapper> mapper;
                mapper->SetInputConnection(sphereSource->GetOutputPort());
                vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
                actor->SetMapper(mapper);
                renderer->AddActor(actor);
                atomActors->push_back(actor);
            }
        }

        #pragma omp parallel for
        for (size_t i = 0; i < frame.all_atoms.size(); i++) {
            const Atom& atom = frame.all_atoms[i];
            vtkSmartPointer<vtkActor> actor = (*atomActors)[i];
            actor->SetPosition(atom.x, atom.y, atom.z);
            set_particle_color(actor, atom);
        }

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

        std::string reading = "Time: " + time_string + " ps TE: " + te_string + " eV KE: " + ke_string + 
                            " eV PE: " + pe_string + " eV Average ke: " + ake_string + " eV";
        reading_actor->SetInput(reading.c_str());
    }
};

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
            std::cout << (pauseAnimation ? "Animation Paused" : "Animation Resumed") << std::endl;
        }
        else if (key == "plus" || key == "equal") {
            playback_speed = std::max(1, playback_speed + 1);
        }
        else if (key == "minus" || key == "underscore") {
            playback_speed = std::max(1, playback_speed - 1);
        }
        else if (key == "r") {
            playback_direction *= -1;
            std::cout << "Playback direction: " << (playback_direction > 0 ? "forward" : "backward") << std::endl;
        }
    }
};

void initialise_atom_actors(const std::vector<Atom>& all_atoms, vtkRenderer* renderer, Settings settings, 
                          std::vector<vtkSmartPointer<vtkActor>>& atom_actors) {
    double atom_radius = settings.get_atom_radius();
    for (const Atom& atom : all_atoms) {
        vtkNew<vtkSphereSource> sphereSource;
        sphereSource->SetCenter(atom.x, atom.y, atom.z);
        sphereSource->SetRadius(atom_radius);
        sphereSource->SetPhiResolution(8);
        sphereSource->SetThetaResolution(8);

        vtkNew<vtkPolyDataMapper> mapper;
        mapper->SetInputConnection(sphereSource->GetOutputPort());

        vtkNew<vtkActor> actor;
        actor->SetMapper(mapper);
        set_particle_color(actor, atom);
        renderer->AddActor(actor);
        atom_actors.push_back(actor);
    }
}

int main(int argc, char *argv[]) {
    std::vector<std::string> arguments(argv, argv + argc);
    Settings settings(arguments);
    settings.print_all_settings();

    std::cout << "Setting up atoms" << std::endl << std::endl;
    std::vector<Atom> all_atoms;
    AtomGenerator atom_gen(settings);

    if (settings.get_atom_mode() == "generate") {
        all_atoms = atom_gen.generate_fcc();
    }

    if (settings.get_add_impact_on() == true && settings.get_bombardment_on() == false) {
        atom_gen.add_impact_atom(all_atoms, settings.get_impact_surface());
    }

    std::cout << "Atoms initialized: " << all_atoms.size() << std::endl;

    SimulationData simData(all_atoms, settings);

    vtkNew<vtkNamedColors> colors;
    vtkNew<vtkRenderer> renderer;
    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    renderWindow->SetWindowName("Molecular Dynamics Playtest");
    renderWindow->SetSize(1280, 720);
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    renderWindowInteractor->SetRenderWindow(renderWindow);

    std::vector<vtkSmartPointer<vtkActor>> atom_actors;
    initialise_atom_actors(all_atoms, renderer, settings, atom_actors);

    vtkSmartPointer<vtkTextActor> reading_actor = vtkSmartPointer<vtkTextActor>::New();
    reading_actor->SetInput("Time: 0.000 s TE: 0.0 KE: 0.0 PE 0.0");
    reading_actor->GetTextProperty()->SetFontSize(24);
    reading_actor->GetTextProperty()->SetColor(1.0, 1.0, 1.0);
    reading_actor->SetPosition(10, 10);
    renderer->AddActor2D(reading_actor);

    vtkNew<TimerCallback> timerCallback;
    timerCallback->simData = &simData;
    timerCallback->renderWindow = renderWindow;
    timerCallback->atomActors = &atom_actors;
    timerCallback->reading_actor = reading_actor;
    timerCallback->renderer = renderer;
    timerCallback->atom_radius = settings.get_atom_radius();

    vtkNew<vtkInteractorStyleTrackballCamera> style;
    renderWindowInteractor->SetInteractorStyle(style);
    renderWindowInteractor->Initialize();
    renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, timerCallback);
    const int timer_interval = 1000 / frame_rate;
    renderWindowInteractor->CreateRepeatingTimer(timer_interval);

    vtkNew<KeyPressCallback> keyPressCallback;
    renderWindowInteractor->AddObserver(vtkCommand::KeyPressEvent, keyPressCallback);

    if (settings.get_parallel_projection_on() == true) {
        renderer->GetActiveCamera()->ParallelProjectionOn();
    }
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Dolly(.5);
    renderer->ResetCameraClippingRange();

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
                    }
                    else if (settings.get_bombardment_mode() == "spread") {
                        atom_gen.add_random_impact_atom(frame.all_atoms, settings.get_impact_surface());
                    }
                    else if (settings.get_bombardment_mode() == "3d_spread") {
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

    renderWindow->Render();
    renderWindowInteractor->Start();

    simulationThread.join();

    return 0;
}