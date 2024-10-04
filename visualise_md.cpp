
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
#include "Type_atom.h"
#include "generate_atoms.h"
#include "animate_atoms.h"
#include "md_driver.h"
#include "file_functions.h"
#include "string_functions.h"



int main(int argc, char *argv[])
{
  std::vector<std::string> arguments(argv, argv+argc);
  Settings settings(arguments);
  settings.print_all_settings();

  std::cout << "Setting up atoms" << std::endl;
  std::vector<Type_atom> all_atoms;
  all_atoms = generate_atom_block(settings);

  // We add an impact atom to the end of the vector
  add_impact_atom(all_atoms, settings);

  // print_atoms(all_atoms);

  if (all_atoms.size() > 12000)
  {
    std::cout << "Too many atoms. Choose smaller dimensions for atom system." << std::endl;
    std::cout << "This limation exists because an overly large atom system is slow to animate." << std::endl;
    return 3;
  }

  // // Simulate molecular dynamics and obtain atom trajectory data
  
  std::cout << "Simulating dynamics" << std::endl;
  std::vector<std::vector<Type_atom>> atom_trajectory_data;
  // atom_trajectory_data.push_back(all_atoms);
  // Type_atom atom;
  atom_trajectory_data = simulate_atom_movement(all_atoms, settings);
  // print_atoms(atom_trajectory_data[0]);
  // print_atoms(atom_trajectory_data[atom_trajectory_data.size() - 1]);

  // // // // Render animation
  std::cout << "Animating" << std::endl;
  animate_atoms(atom_trajectory_data, settings);
  std::cout << "End of program reached" << std::endl;

  return 0;
}