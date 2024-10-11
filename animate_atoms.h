#ifndef __animate_atoms_h
#define __animates_atoms_h
#include "AtomAnimator.h"
#include <vector>
#include <iostream>

#include "Settings.h"
#include "Atom.h"
#include "generate_atoms.h"
#include "md_driver.h"




int animate_atoms(std::vector<Atom> all_atoms, Settings settings);


int animate_atoms(std::vector<Atom> all_atoms, Settings settings)
{
    vtkNew<vtkNamedColors> colors;
    // Create and actor for each atom
    std::vector<vtkSmartPointer<vtkActor>> actors;

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
        actor->GetProperty()->SetColor(colors->GetColor3d("White").GetData());
        actors.push_back(actor);
    }

    // A renderer and render window.
    vtkNew<vtkRenderer> renderer;
    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    renderWindow->SetWindowName("Molecular Dynamics Playtest");
    renderWindow->SetSize(1280, 720);

    // An interactor.
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    renderWindowInteractor->SetRenderWindow(renderWindow);


    // Add the actors to the scene.
    for (int i = 0; i < actors.size(); i++)
    {
        renderer->AddActor(actors[i]);
    }

    renderer->SetBackground(colors->GetColor3d("DarkSlateGray").GetData());

    // Render
    renderWindow->Render();

    if (settings.get_parallel_projection_on() == true)
    {
        renderer->GetActiveCamera()->ParallelProjectionOn();
    }
    
    vtkNew<vtkInteractorStyleTrackballCamera> style;

    renderWindowInteractor->SetInteractorStyle(style);
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Dolly(.5);
    renderer->ResetCameraClippingRange();

    // Create an Animation Scene
    vtkNew<vtkAnimationScene> scene;
    int step_duration = settings.get_animation_step_duration();
    int start_time = 0;
    int end_time = start_time + step_duration;

    vtkLogF(INFO, "sequence mode");
    scene->SetModeToSequence();

    scene->SetLoop(0);
    scene->SetFrameRate(5);
    scene->AddObserver(vtkCommand::AnimationCueTickEvent, renderWindow.GetPointer(),
                        &vtkWindow::Render);

    // Set up atom animators
    // For some reason, the animations don't function properly if I use a
    // vector to store the animators.
    AtomAnimator* animators = new AtomAnimator[all_atoms.size()];
    for (int i = 0; i < all_atoms.size(); i++)
    {
        AtomAnimator animator;
        animator.SetActor(actors[i]);
        animators[i] = animator;
    }


    std::vector<Atom> current_snapshot = all_atoms;
    std::vector<Atom> next_snapshot;
    int snapshot_interval = settings.get_simulation_history_interval();
    int snapshot_count = 0;
    double time = 0;
    double current_kinetic = 0;
    double current_potential = 0;
    while (true)
    {
        std::cout << "Snapshot:" << snapshot_count << std::endl;
        // Set up start position
        for (int i = 0; i < current_snapshot.size(); i++)
        {
            animators[i].SetStartPosition(vtkVector3d(current_snapshot[i].x, current_snapshot[i].y, current_snapshot[i].z));
        }
        scene->SetStartTime(start_time);
        scene->SetEndTime(end_time);
        // Create an Animation Cue for each actor
        vtkNew<vtkAnimationCue> cue1;
        cue1->SetStartTime(start_time);
        cue1->SetEndTime(end_time);
        scene->AddCue(cue1);
        // Update start and end time for the next animation step
        start_time = end_time;
        end_time += step_duration;

        // Create the next snapshot
        next_snapshot = simulate_atom_movement(current_snapshot, settings, time);
        // Set up the animation for each actor in the animation step
        for (int i = 0; i < next_snapshot.size(); i++)
        {
            animators[i].SetEndPosition(vtkVector3d(next_snapshot[i].x, next_snapshot[i].y, next_snapshot[i].z));
            animators[i].AddObserversToCue(cue1);
        }

        // Play the Scene
        scene->Play();
        scene->Stop();

        current_snapshot = next_snapshot;
    }



    // Begin mouse interaction.
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;


}

#endif
