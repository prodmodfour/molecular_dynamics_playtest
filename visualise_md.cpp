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
#include "Settings.h"
#include "Atom.h"
#include "generate_atoms.h"
#include "animate_atoms.h"
#include "simulation.h"
#include "file_functions.h"
#include "SimulationData.h"
#include <thread>
#include <mutex>
#include <chrono>

bool pauseAnimation = false;  // Global pause flag for controlling animation
int timesteps_animated_per_second = 100;
const int frame_rate = 30;

void set_particle_color(vtkSmartPointer<vtkActor> actor, const Atom& atom)
{
    double ratio = std::clamp(atom.ke / atom.reference_ke,  0.0, 1.0);
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
            if (!pauseAnimation) {  
                if (simData->ready()) 
                {
                    Frame frame = simData->get_next_frame();
                    updateSceneWithFrame(frame);
                    
                }
                renderWindow->Render();
            }
        }

        SimulationData* simData;
        vtkRenderWindow* renderWindow;
        std::vector<vtkSmartPointer<vtkActor>>* atomActors;

        // Function to update the scene (user-defined)
        void updateSceneWithFrame(const Frame& frame) 
        {
            for (size_t i = 0; i < frame.all_atoms.size(); ++i) 
            {

                Atom& atom = frame.all_atoms[i];
                vtkSmartPointer<vtkActor> actor = (*atomActors)[i]; 
                actor->SetPosition(atom.x, atom.y, atom.z); 
                set_particle_color(actor, atom);
            }
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
        }
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

    std::cout << "Atoms initialized: " << all_atoms.size() << std::endl;

    // Create the simulation data
    SimulationData simData;

    // Create the VTK components for rendering
    vtkNew<vtkNamedColors> colors;
    vtkNew<vtkRenderer> renderer;
    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    renderWindowInteractor->SetRenderWindow(renderWindow);

    std::vector<vtkSmartPointer<vtkActor>> atom_actors;
    initialise_atom_actors(all_atoms, renderer, settings, atom_actors);

    // Set up the timer-based callback for non-blocking animation
    vtkNew<TimerCallback> timerCallback;
    timerCallback->simData = &simData;
    timerCallback->renderWindow = renderWindow;
    timerCallback->atomActors = &atom_actors;
    
    // Create a repeating timer to call the animation update every 100ms
    renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, timerCallback);
    renderWindowInteractor->CreateRepeatingTimer(100);  // 100 ms interval for animation updates

    // Set up the keypress callback for pausing
    vtkNew<KeyPressCallback> keyPressCallback;
    renderWindowInteractor->AddObserver(vtkCommand::KeyPressEvent, keyPressCallback);

    // Start a separate thread for molecular dynamics simulation (infinite loop)
    std::thread simulationThread([&simData]() {
        while (true) 
        {
            int timestepsPerFrame = timestepsPerSecond / frameRate;
            Frame new_frame(/* Generate frame with atom data, energy, etc. */);
            simData.add_frame(new_frame);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / frame_rate));  // Simulate simulation step time
        }
    });

    vtkNew<vtkInteractorStyleTrackballCamera> style;
    renderWindowInteractor->SetInteractorStyle(style);
    if (settings.get_parallel_projection_on() == true)
    {
        renderer->GetActiveCamera()->ParallelProjectionOn();
    }
    
    vtkNew<vtkInteractorStyleTrackballCamera> style;
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Dolly(.5);
    renderer->ResetCameraClippingRange();

    // Start the interaction loop (keeps window interactive while running animation)
    renderWindow->Render();
    renderWindowInteractor->Start();

    // Join the simulation thread after animation ends (optional since it runs forever)
    simulationThread.join();

    return 0;
}
