
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

#define EV_TO_J_PER_MOLE 96400.0
#define J_PER_MOLE_TO_EV 1.037e-5
#define SCALING 0.01

int main(int argc, char *argv[])
{

  if(argc != 5)
  {
    printf("Incorrect number of input arguments.\n");
    printf("First 3 arguments must be integers (Cubes in x, Cubes in y, Cubes in z)\n");
    printf("The fourth argument must be an integer (Duration of each animation step)\n");
    printf("The bigger this number, the slower the animation\n");
    printf("e.g. '2 2 2 5'/n");

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
  int steps = 10;
  std::vector<std::vector<Type_atom>> atom_trajectory_data;
  atom_trajectory_data.reserve(steps);

  // This is the first from of the animation
  atom_trajectory_data[0] = all_atoms;
  print_atoms(atom_trajectory_data[0]);
  

  for (int j = 1; j < steps; j++)
  {
    for (int i = 0; i < all_atoms.size(); i++)
    {
      all_atoms[i].x +=1;
      all_atoms[i].y +=1;
      all_atoms[i].z +=1;
    }
    atom_trajectory_data[j] = all_atoms;
  }


  // Render animation
  int step_duration = std::stoi(argv[4]);
  animate_atoms(atom_trajectory_data, step_duration);
  std::cout << "End of program reached" << std::endl;

  
}