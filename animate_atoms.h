#ifndef __animate_atoms_h
#define __animates_atoms_h
#include "AtomAnimator.h"
#include <vector>
#include <iostream>

#include "Settings.h"
#include "Type_atom.h"
#include "generate_atoms.h"





int animate_atoms(std::vector<std::vector<Type_atom>> &atom_trajectory_data, Settings settings);



int animate_atoms(std::vector<std::vector<Type_atom>> &atom_trajectory_data, Settings settings)
{
    vtkNew<vtkNamedColors> colors;
    // Create and actor for each atom
    std::vector<vtkSmartPointer<vtkActor>> actors;

    double atom_radius = settings.get_atom_radius(); //Angstroms
    for (int i = 0; i < atom_trajectory_data[0].size(); i++)
    {
        vtkNew<vtkSphereSource> sphereSource;

        sphereSource->SetCenter(atom_trajectory_data[0][i].x, atom_trajectory_data[0][i].y, atom_trajectory_data[0][i].z);


        sphereSource->SetRadius(atom_radius);

        vtkNew<vtkPolyDataMapper> mapper;
        mapper->SetInputConnection(sphereSource->GetOutputPort());

        vtkNew<vtkActor> actor;
        actor->SetMapper(mapper);
        actor->GetProperty()->SetColor(colors->GetColor3d("White").GetData());

        if (i == (atom_trajectory_data[0].size() - 1))
        {
        actor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());
        }

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
    AtomAnimator* animators = new AtomAnimator[atom_trajectory_data[0].size()];
    for (int i = 0; i < atom_trajectory_data[0].size(); i++)
    {
        AtomAnimator animator;
        animator.SetActor(actors[i]);
        animators[i] = animator;
    }

    // renderer->GetActiveCamera()->ParallelProjectionOn();
    // double parallel_scale;
    // parallel_scale = renderer->GetActiveCamera()->GetParallelScale();
    // std::cout << parallel_scale << std::endl;

    std::cout << "Total animation steps " << atom_trajectory_data.size() << std::endl;
    for (int step = 0; step < atom_trajectory_data.size() - 1; step++)
    {
        // Set up start position
        for (int i = 0; i < atom_trajectory_data[step].size(); i++)
        {
            animators[i].SetStartPosition(vtkVector3d(atom_trajectory_data[step][i].x, atom_trajectory_data[step][i].y, atom_trajectory_data[step][i].z));
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

        // Set up the animation for each actor in the animation step
        for (int i = 0; i < atom_trajectory_data[step].size(); i++)
        {
            animators[i].SetEndPosition(vtkVector3d(atom_trajectory_data[step + 1][i].x, atom_trajectory_data[step + 1][i].y, atom_trajectory_data[step + 1][i].z));
            animators[i].AddObserversToCue(cue1);
        }

        // Play the Scene
        

        scene->Play();
        scene->Stop();
        std::cout << "Step "<< step << std::endl;
        

        if (step == atom_trajectory_data.size() - 1)
        {
            continue;

        }
        // Set up start positions for the next animation step
        // for (int i = 0; i < atom_trajectory_data[step].size(); i++)
        // {
        //     animators[i].SetStartPosition(vtkVector3d(atom_trajectory_data[step][i].x, atom_trajectory_data[step][i].y, atom_trajectory_data[step][i].z));
        // }
    }



    // Begin mouse interaction.
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;


}

#endif
