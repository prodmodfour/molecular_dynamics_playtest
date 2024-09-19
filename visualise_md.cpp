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

  if (all_atoms.size() > 12000)
  {
    std::cout << "Too many atoms. Choose smaller dimensions for atom system." << std::endl;
    return 3;
  }

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

  // Render
  renderWindow->Render();

  vtkNew<vtkInteractorStyleTrackballCamera> style;

  renderWindowInteractor->SetInteractorStyle(style);
  renderer->ResetCamera();
  renderer->GetActiveCamera()->Dolly(.5);
  renderer->ResetCameraClippingRange();

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

  scene->AddObserver(vtkCommand::AnimationCueTickEvent, renderWindow.GetPointer(),
                     &vtkWindow::Render);

  // Set up animation for each atom
  AtomAnimator* animators = new AtomAnimator[all_atoms.size()];
  int total_timesteps = 10;
  double next_x, next_y, next_z, current_x, current_y, current_z;
  int start_time = 0;
  int step_duration = 5;
  int end_time = start_time + step_duration;

  for (int j = 1; j < total_timesteps; j++)
  {
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
    
    for (int i = 0; i < all_atoms.size(); i++)
    {
      AtomAnimator animator1;

      current_x = all_atoms[i].x + j - 1;
      current_y = all_atoms[i].y + j - 1;
      current_z = all_atoms[i].z + j -1;
      animator1.SetStartPosition(vtkVector3d(current_x, current_y, current_z));

      // Find the position that the atom will be at the end of the animation step
      next_x = all_atoms[i].x + j;
      next_y = all_atoms[i].y + j;
      next_z = all_atoms[i].z + j;
      animator1.SetEndPosition(vtkVector3d(next_x, next_y, next_z));
      animator1.SetActor(actors[i]);
      animators[i] = animator1;
      animators[i].AddObserversToCue(cue1);
    }

    // Create Cue observer.
    scene->Play();
    scene->Stop();

  }


  // Begin mouse interaction.
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}