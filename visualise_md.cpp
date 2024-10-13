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
#include "generate_atoms.h"

#include "simulation.h"
#include "file_functions.h"
#include "SimulationData.h"
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <cmath>

bool pauseAnimation = false;  // Global pause flag for controlling animation
int playback_speed = 1;
const int frame_rate = 30;
int playback_direction = 1; // 1 for forwards, -1 for backward

void set_particle_color(vtkSmartPointer<vtkActor> actor, const Atom& atom)
{
    double ratio = atom.ke / atom.reference_ke;
    if (ratio > 1)
    {
        ratio = 1;
    }
    else if (ratio < 0)
    {
        ratio = 0;
    }

    double red = ratio;
    double blue = 1.0 - ratio;
    actor->GetProperty()->SetColor(red, 0.0, blue);
    return;

}

// Timer callback for non-blocking animation
class TimerCallback : public vtkCommand 
{
    public:
        static TimerCallback* New() 
        {
            return new TimerCallback;
        }

        void Execute(vtkObject* caller, unsigned long eventId, void* callData) override 
        {
            if (!pauseAnimation) 
            {  
                for (int i = 0; i < playback_speed; i++)
                {
                    bool success;
                    if (playback_direction > 0)
                    {
                        success = simData->move_forward();
                        if (!success)
                        {
                            // Wait for new frames or pause if simulation has ended
                            break;
                        }
                    }
                    else
                    {
                        success = simData->move_backward();
                        if (!success)
                        {
                            break;
                        }
                    }
                    Frame frame = simData->get_current_frame();
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

        // Function to update the scene (user-defined)
        void updateSceneWithFrame(const Frame& frame) 
        {
            size_t numAtoms = frame.all_atoms.size();
            size_t numActors = atomActors->size();

            if (numAtoms > numActors)
            {
                for (size_t i = numActors; i < numAtoms; ++i)
                {
                    // Create a new actor for the new atom
                    vtkSmartPointer<vtkActor> actor = create_atom_actor(frame.all_atoms[i]);
                    renderer->AddActor(actor);
                    atomActors->push_back(actor);
                }
            }
            for (size_t i = 0; i < frame.all_atoms.size(); i++) 
            {
                Atom atom = frame.all_atoms[i];
                vtkSmartPointer<vtkActor> actor = (*atomActors)[i]; 
                actor->SetPosition(atom.x, atom.y, atom.z); 
                set_particle_color(actor, atom);


            }
            std::ostringstream oss;
            // Set time to 3 decimal place
            oss << std::fixed << std::setprecision(3) << frame.time;
            std::string time_string = oss.str();

            std::ostringstream oss1;

            // Set energies to 4 sig fig

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
            oss1 << std::setprecision(4) << (average_ke);
            std::string ake_string = oss1.str();

            std::string reading = "Time: " + time_string + " ps " " TE: " + te_string + " eV " + " KE: " + ke_string + " eV "  + " PE: "  + pe_string + " eV" + " Average ke: " + ake_string + " eV";
            reading_actor->SetInput(reading.c_str());
        }

        vtkSmartPointer<vtkActor> create_atom_actor(const Atom& atom)
        {
            vtkNew<vtkSphereSource> sphereSource;
            sphereSource->SetCenter(atom.x, atom.y, atom.z);
            sphereSource->SetRadius(atom_radius); // You need to have access to atom_radius

            vtkNew<vtkPolyDataMapper> mapper;
            mapper->SetInputConnection(sphereSource->GetOutputPort());

            vtkNew<vtkActor> actor;
            actor->SetMapper(mapper);
            set_particle_color(actor, atom);

            return actor;
        }
};

void initialise_atom_actors(const std::vector<Atom>& all_atoms, vtkRenderer* renderer, Settings settings, std::vector<vtkSmartPointer<vtkActor>>& atom_actors)
{
    double atom_radius = settings.get_atom_radius(); //Angstroms
    for (int i = 0; i < all_atoms.size(); i++)
    {
        vtkNew<vtkSphereSource> sphereSource;

        sphereSource->SetCenter(all_atoms[i].x, all_atoms[i].y, all_atoms[i].z);


        sphereSource->SetRadius(atom_radius);

        vtkNew<vtkPolyDataMapper> mapper;
        mapper->SetInputConnection(sphereSource->GetOutputPort());

        vtkNew<vtkActor> actor;
        actor->SetMapper(mapper);
        set_particle_color(actor, all_atoms[i]);
        renderer->AddActor(actor);
        atom_actors.push_back(actor);
    }
}



// Function to handle keyboard events
class KeyPressCallback : public vtkCommand {
public:
    static KeyPressCallback* New() {
        return new KeyPressCallback;
    }

    virtual void Execute(vtkObject* caller, unsigned long eventId, void* callData) override 
    {
        vtkRenderWindowInteractor* interactor = static_cast<vtkRenderWindowInteractor*>(caller);
        std::string key = interactor->GetKeySym();

        if (key == "space") {  
            pauseAnimation = !pauseAnimation;
            if (pauseAnimation) 
            {
                std::cout << "Animation Paused" << std::endl;
            } 
            else 
            {
                std::cout << "Animation Resumed" << std::endl;
            }
        }
        else if (key == "plus" || key == "equal")
        {
            adjustplayback_speed(1); // Increase playback speed
        }
        else if (key == "minus" || key == "underscore")
        {
            adjustplayback_speed(-1); // Decrease playback speed
        }
        else if (key == "r") 
        {
            playback_direction *= -1; // Reverse the playback direction
            if (playback_direction > 0) 
            {
                std::cout << "Playback direction: forward" << std::endl;
            } 
            else 
            {
                std::cout << "Playback direction: backward" << std::endl;
            }
        }

    }
    
    void adjustplayback_speed(int delta)
    {
        playback_speed = std::max(1, playback_speed + delta); 
    }
};

int main(int argc, char *argv[])
{
    std::vector<std::string> arguments(argv, argv + argc);
    Settings settings(arguments);
    settings.print_all_settings();

    std::cout << "Setting up atoms" << std::endl << std::endl;
    std::vector<Atom> all_atoms;

    if (settings.get_atom_mode() == "generate")
    {
        all_atoms = generate_fcc(settings);
        write_atoms_to_file(all_atoms);
    }
    else if (settings.get_atom_mode() == "from_file")
    {
        std::string filename = settings.get_atom_filename();
        std::cout << "Atom filename: " << filename << std::endl << std::endl;;
        all_atoms = read_atoms_from_file(filename);
    }

    // We add an impact atom to the end of the vector

    if (settings.get_add_impact_on() == true  && settings.get_bombardment_on() == false)
    {
        add_impact_atom(all_atoms, settings, settings.get_impact_surface());
    }

    std::cout << "Atoms initialized: " << all_atoms.size() << std::endl;

    // Create the simulation data
    SimulationData simData(all_atoms, settings);

    // Create the VTK components for rendering
    vtkNew<vtkNamedColors> colors;
    vtkNew<vtkRenderer> renderer;
    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    renderWindow->SetWindowName("Molecular Dynamics Playtest");
    renderWindow->SetSize(1280, 720);
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    renderWindowInteractor->SetRenderWindow(renderWindow);

    
    // Initialise actors
    std::vector<vtkSmartPointer<vtkActor>> atom_actors;
    initialise_atom_actors(all_atoms, renderer, settings, atom_actors);

    vtkSmartPointer<vtkTextActor> reading_actor = vtkSmartPointer<vtkTextActor>::New();
    reading_actor->SetInput("Time: 0.000 s TE: 0.0 KE: 0.0 PE 0.0");
    reading_actor->GetTextProperty()->SetFontSize(24);
    reading_actor->GetTextProperty()->SetColor(1.0, 1.0, 1.0);
    reading_actor->SetPosition(10, 10);

    // Set up the timer-based callback for non-blocking animation
    vtkNew<TimerCallback> timerCallback;
    timerCallback->simData = &simData;
    timerCallback->renderWindow = renderWindow;
    timerCallback->atomActors = &atom_actors;
    timerCallback->reading_actor = reading_actor;
    timerCallback->renderer = renderer;
    timerCallback->atom_radius = settings.get_atom_radius();
    renderer->AddActor2D(reading_actor);

    
    vtkNew<vtkInteractorStyleTrackballCamera> style;
    renderWindowInteractor->SetInteractorStyle(style);
    
    renderWindowInteractor->Initialize();

    renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, timerCallback);
    const int timer_interval = 1000 / frame_rate;
    renderWindowInteractor->CreateRepeatingTimer(timer_interval);  

    // Set up the keypress callback for pausing
    vtkNew<KeyPressCallback> keyPressCallback;
    renderWindowInteractor->AddObserver(vtkCommand::KeyPressEvent, keyPressCallback);


    
    if (settings.get_parallel_projection_on() == true)
    {
        renderer->GetActiveCamera()->ParallelProjectionOn();
    }
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Dolly(.5);
    renderer->ResetCameraClippingRange();

    // Start a separate thread for molecular dynamics simulation (infinite loop)
    std::thread simulationThread([&simData, &settings]() 
    {
        int timesteps_per_frame = 5;
        double next_bombardment_time = settings.get_bombardment_interval();
        while (true) 
        {   

            if (simData.buffer_full())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            Frame frame = simData.get_latest_frame();

            if (settings.get_bombardment_on())
            {
                if (frame.time >= next_bombardment_time)
                {
                    if (settings.get_bombardment_mode() == "consistent")
                    {
                        add_impact_atom(frame.all_atoms, settings, settings.get_impact_surface());
                    }
                    else if (settings.get_bombardment_mode() == "spread")
                    {
                        add_random_impact_atom(frame.all_atoms, settings, settings.get_impact_surface());
                    }
                    else if (settings.get_bombardment_mode() == "3d_spread")
                    {
                        add_impact_atom_random_surface(frame.all_atoms, settings);
                    }


                    std::cout << "Added impact atom at " << frame.time << std::endl;
                    next_bombardment_time += settings.get_bombardment_interval();
                }
            }
            frame = create_next_frame(frame, settings, timesteps_per_frame);
            simData.add_frame(frame);
        }
    });

    // Start the interaction loop (keeps window interactive while running animation)
    renderWindow->Render();
    renderWindowInteractor->Start();






    // Join the simulation thread after animation ends (optional since it runs forever)
    simulationThread.join();

    return 0;
}
