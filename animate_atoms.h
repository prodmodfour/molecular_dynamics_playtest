#include "Type_atom.h"
#include "AtomAnimator.h"
#include <vector>
#include <vtkAnimationScene.h>
#include <vtkAnimationCue.h>

void animate_atoms(std::vector<std::vector<Type_atom>> atom_trajectory_data, vtkAnimationScene scene, AtomAnimator* animators)
{
    int time = 0;
    for (int step = 0; step < atom_trajectory_data.size(); step++)
    {
        // Create an Animation Cue for each actor
        vtkNew<vtkAnimationCue> cue1;
        cue1->SetStartTime(time);
        cue1->SetEndTime(time + 1);
        scene->AddCue(cue1);
        for (int j = 0; j < atom_trajectory_data[j].size(); j++)
        {


            // Set up the animation for each actor in the animation step
            for (int atom_index = 0; atom_index < all_atoms.size(); i++)
            {
                AtomAnimator animator1;
                
                animator1.SetEndPosition(vtkVector3d(, all_atoms[i].y + j, all_atoms[i].z + j));
                animator1.SetActor(actors[i]);
                animators[i] = animator1;
                animators[i].AddObserversToCue(cue1);
            }
        }
        time++;

        // Play the Scene
        scene->Play();
        scene->Stop();
    }


}