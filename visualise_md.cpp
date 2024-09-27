
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



int main(int argc, char *argv[])
{
  // Get input arguments
  if(argc != 7 && argc != 1)
  {
    printf("Incorrect number of input arguments.\n");
    printf("First 3 arguments must be integers (Cubes in x, Cubes in y, Cubes in z)\n");
    printf("The fourth argument must be an integer (Duration of each animation step)\n");
    printf("The bigger this number, the slower the animation\n");
    std::cout << "The fifth argument should be the timstep size for the simulation" << std::endl;
    std::cout << "The sixth argument should be the number of timesteps in the simulation" << std::endl;
    printf("e.g. '2 2 2 100 0.001 1000'/n");  

    return 0;
  }
  Settings settings(argc, argv);


  // Generate Atom system based on user input
  

  std::vector<Type_atom> all_atoms = generate_atom_block(settings);
  
  // We add an impact atom to the end of the vector
  add_impact_atom(all_atoms, settings);

  print_atoms(all_atoms);

  if (all_atoms.size() > 12000)
  {
    std::cout << "Too many atoms. Choose smaller dimensions for atom system." << std::endl;
    std::cout << "This limation exists because an overly large atom system is slow to animate." << std::endl;
    return 3;
  }

  // // Simulate molecular dynamics and obtain atom trajectory data
   
  std::vector<std::vector<Type_atom>> atom_trajectory_data;

  // Type_atom atom;
  atom_trajectory_data = simulate_atom_movement(all_atoms, settings);
  std::cout << atom_trajectory_data.size() << std::endl;
  // print_atoms(atom_trajectory_data[0]);
  print_atoms(atom_trajectory_data[atom_trajectory_data.size() - 1]);

  // // // // Render animation
  animate_atoms(atom_trajectory_data, settings);
  std::cout << "End of program reached" << std::endl;

  return 0;
}