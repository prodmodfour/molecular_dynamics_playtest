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

#include "generate_block.h"

// void print_atoms(Type_atoms *Atoms, int number_atoms)
// {
//     // Print all atoms
//     for (int i = 0; i < number_atoms; i++)
//     {
//         printf("Index %d x %f y %f z %f\n", i, Atoms[i].x, Atoms[i].y, Atoms[i].z);
//     }
// }

int main(int, char*[])
{
  // There are 3 arguments to be passed to the program
  // x, y, and z
  // These are the number of unit cubes in the x, y and z direction
  

  // if (argc != 4)
  // {
  //     printf("Incorrect number of arguments. Please enter 3 integers (Number of unit cubes in the X, Y and Z directions)\n");
  //     return 0;
  // }

  // int cubes_in_x = std::stoi(argv[1]);
  // int cubes_in_y = std::stoi(argv[2]);
  // int cubes_in_z = std::stoi(argv[3]);

  int cubes_in_x = 2;
  int cubes_in_y = 2;
  int cubes_in_z = 2;
  std::vector<Type_atoms> atoms;
  // Type_atoms *Atoms;

  // int number_atoms = determine_number_atoms(cubes_in_x, cubes_in_y, cubes_in_z);
  // Atoms = new Type_atoms[number_atoms];
  // // We initialise the impact atom coordinates to 0
  // // The impact atom is always the last atom in the array
  // Atoms[number_atoms].x = 0.0;
  // Atoms[number_atoms].y = 0.0;
  // Atoms[number_atoms].z = 0.0;
  // printf("Total Atoms %d\n", number_atoms);
  generate_block(atoms, cubes_in_x, cubes_in_y, cubes_in_z);
  // print_atoms(Atoms, number_atoms);



  vtkNew<vtkNamedColors> colors;

  std::vector<vtkSmartPointer<vtkActor>> actors;
  for (int i = 0; i < (10); i++)
  {
    vtkNew<vtkSphereSource> sphereSource;

    sphereSource->SetCenter(i, 0, 0);

    double copper_atom_radius = 1.28; //Angstroms
    sphereSource->SetRadius(copper_atom_radius);

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(sphereSource->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(colors->GetColor3d("MistyRose").GetData());

    actors.push_back(actor);
  }



  // A renderer and render window.
  vtkNew<vtkRenderer> renderer;
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  renderWindow->SetWindowName("Molecular Dynamics Playtest");

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

  // Begin mouse interaction.
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}