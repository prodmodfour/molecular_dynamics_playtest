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

int main(int, char*[])
{

  


  int cubes_in_x = 4;
  int cubes_in_y = 4;
  int cubes_in_z = 4;
  std::vector<Type_atoms> atoms = generate_block(cubes_in_x, cubes_in_y, cubes_in_z);




  // We add an impact atom to the end of the vector
  // This atom is initialised at (0, 0, 0) but will change position to 
  // 1 Angstrom away from the centre of the top x-y surface of the block
  Type_atoms atom;
  atom.x = 0.0;
  atom.y = 0.0;
  atom.z = 0.0;
  atoms.push_back(atom);
  printf("Total Atoms %zd\n", atoms.size());
  // print_atoms(atoms);




  vtkNew<vtkNamedColors> colors;

  std::vector<vtkSmartPointer<vtkActor>> actors;
  for (int i = 0; i < atoms.size(); i++)
  {
    vtkNew<vtkSphereSource> sphereSource;

    sphereSource->SetCenter(atoms[i].x, atoms[i].y, atoms[i].z);

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