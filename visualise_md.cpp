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


#include <vector>
#include <Windows.h>

#include "generate_atoms.h"
#include "AtomAnimator.h"

#define EV_TO_J_PER_MOLE 96400.0
#define J_PER_MOLE_TO_EV 1.037e-5
#define SCALING 0.01

int main(int argc, char *argv[])
{

  if(argc != 5)
  {
    printf("Incorrect number of input arguments.\n");
    printf("First 3 arguments must be integers (Cubes in x, Cubes in y, Cubes in z)\n");
    printf("The fourth argument must be a flag that determines the animation type, either real time of sequence\n");
    printf("e.g. '2 2 2 -real' or '1 2 3 -sequence'/n");

    return 0;
  }

  block_dimensions cubes_in;

  cubes_in.x = std::stoi(argv[1]);
  cubes_in.y = std::stoi(argv[2]); 
  cubes_in.z = std::stoi(argv[3]); 
  std::vector<Type_atom> all_atoms = generate_atom_block(cubes_in);
  


  // We add an impact atom to the end of the vector
  // This atom is initialised 1 Angstrom above the centre of the top x-y surface of the block
  double z_offset = 3;
  double applied_energy = 1000;
  add_impact_atom(all_atoms, z_offset, applied_energy, cubes_in);
  print_atoms(all_atoms);


  vtkNew<vtkNamedColors> colors;

  std::vector<vtkSmartPointer<vtkActor>> actors;
  double copper_atom_radius = 1.28; //Angstroms
  for (int i = 0; i < all_atoms.size(); i++)
  {
    vtkNew<vtkSphereSource> sphereSource;

    sphereSource->SetCenter(all_atoms[i].x, all_atoms[i].y, all_atoms[i].z);

    
    sphereSource->SetRadius(copper_atom_radius);

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(sphereSource->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(colors->GetColor3d("MistyRose").GetData());

    if (i == (all_atoms.size() - 1))
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

  // Create an Animation Scene
  vtkNew<vtkAnimationScene> scene;

  if (strcmp(argv[4], "-real") == 0)
  {
    vtkLogF(INFO, "real-time mode");
    scene->SetModeToRealTime();
  }
  else if (strcmp(argv[4], "-sequence") == 0)
  {
    vtkLogF(INFO, "sequence mode");
    scene->SetModeToSequence();
  }
  else
  {
    std::cout << "Incorrect Flag" << std::endl;
    return 2;
  }
  scene->SetLoop(0);
  scene->SetFrameRate(5);
  scene->SetStartTime(0);
  scene->SetEndTime(5);
  scene->AddObserver(vtkCommand::AnimationCueTickEvent, renderWindow.GetPointer(),
                     &vtkWindow::Render);

  // Set up animation for each atom
  // std::vector<vtkSmartPointer<vtkAnimationCue>> animation_cues;
  // std::vector<AtomAnimator> animators;

  // Create an Animation Cue for each actor
  vtkNew<vtkAnimationCue> cue1;
  cue1->SetStartTime(1);
  cue1->SetEndTime(10);
  scene->AddCue(cue1);

  vtkNew<vtkAnimationCue> cue2;
  cue2->SetStartTime(1);
  cue2->SetEndTime(10);
  scene->AddCue(cue2);

  // Create an ActorAnimator for each actor;
  AtomAnimator animator1;
  animator1.SetEndPosition(vtkVector3d(all_atoms[0].x + 10, all_atoms[0].y + 10, all_atoms[0].z + 10));
  animator1.SetActor(actors[0]);
  animator1.AddObserversToCue(cue1);

  AtomAnimator animator2;
  animator2.SetEndPosition(vtkVector3d(all_atoms[1].x + 10, all_atoms[1].y + 10, all_atoms[1].z + 10));
  animator2.SetActor(actors[1]);
  animator2.AddObserversToCue(cue2);


  // for (int i = 0; i < (10); i++)
  // {
  //   // Create the animation cue
  //   vtkNew<vtkAnimationCue> cue;
  //   cue->SetStartTime(i);
  //   cue->SetEndTime(i + 5);
  //   animation_cues.push_back(cue);
  //   scene->AddCue(animation_cues[i]);

  //   // Create Atom Animator
  //   AtomAnimator animateAtom;
  //   animators.push_back(animateAtom);
  //   animators[i].SetEndPosition(vtkVector3d(all_atoms[i].x + 1, all_atoms[i].y + 1, all_atoms[i].z + 1));
  //   animators[i].SetActor(actors[i]);

  //   // Create Cue observer.
  //   vtkNew<vtkAnimationCueObserver> observer;
  //   observer->Renderer = renderer;
  //   observer->Animator = &animators[i];
  //   observer->RenWin = renderWindow;

  //   animation_cues[i]->AddObserver(vtkCommand::StartAnimationCueEvent, observer);
  //   animation_cues[i]->AddObserver(vtkCommand::EndAnimationCueEvent, observer);
  //   animation_cues[i]->AddObserver(vtkCommand::AnimationCueTickEvent, observer);


  // }

  // Render
  renderWindow->Render();

  vtkNew<vtkInteractorStyleTrackballCamera> style;

  renderWindowInteractor->SetInteractorStyle(style);
  renderer->ResetCamera();
  renderer->GetActiveCamera()->Dolly(.5);
  renderer->ResetCameraClippingRange();

  // Create Cue observer.
  scene->Play();
  scene->Stop();

  // Begin mouse interaction.
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}