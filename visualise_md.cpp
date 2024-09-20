
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

#include "Type_atom.h"
#include "generate_atoms.h"
#include "animate_atoms.h"
#include "md_driver.h"

#define EV_TO_J_PER_MOLE 96400.0
#define J_PER_MOLE_TO_EV 1.037e-5
#define SCALING 0.01

int main(int argc, char *argv[])
{

  if(argc != 6)
  {
    printf("Incorrect number of input arguments.\n");
    printf("First 3 arguments must be integers (Cubes in x, Cubes in y, Cubes in z)\n");
    printf("The fourth argument must be an integer (Duration of each animation step)\n");
    printf("The bigger this number, the slower the animation\n");
    std::cout << "The fifth argument should be the number of timesteps in the simulation" << std::endl;
    printf("e.g. '2 2 2 5 1000'/n");

    return 0;
  }

  // Generate Atom system based on user input
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

  if (all_atoms.size() > 12000)
  {
    std::cout << "Too many atoms. Choose smaller dimensions for atom system." << std::endl;
    return 3;
  }

  // Simulate molecular dynamics and obtain atom trajectory data
  int number_timesteps = std::stoi(argv[5]);
  std::vector<std::vector<Type_atom>> atom_trajectory_data;

  atom_trajectory_data = simulate_atom_movement(all_atoms, number_timesteps, 100);
  std::cout << atom_trajectory_data.size() << std::endl;
  print_atoms(atom_trajectory_data[0]);
  print_atoms(atom_trajectory_data[atom_trajectory_data.size() - 1]);

  // // Render animation
  int step_duration = std::stoi(argv[4]);
  animate_atoms(atom_trajectory_data, step_duration);
  std::cout << "End of program reached" << std::endl;

  return 0;
}