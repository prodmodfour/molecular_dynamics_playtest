#include <vector>

typedef struct {
   double x, y, z;
   double vx, vy, vz;
   double fx, fy, fz;
} Type_atom;


// Past the simulation phase, values for forces and velocities are not required
// We then switch to this smaller struct in order to save space. 
typedef struct {
   double x, y, z;
} Type_atom_coordinates;

void print_trimmed_atom(Type_atom_coordinates atom)
{
   std::cout << atom.x << std::endl;
   std::cout << atom.y << std::endl;
   std::cout << atom.z << std::endl;
}

std::vector<Type_atom_coordinates> trim_atom_data(std::vector<Type_atom> atom_data)
{
   std::vector<Type_atom_coordinates> trimmed_atom_data;
   trimmed_atom_data.reserve(atom_data.size());
   // Convert a vector of Type_atom to Type_atom_coordinates
   for (int i = 0; i < atom_data.size(); i++)
   {
      trimmed_atom_data[i].x = atom_data[i].x;
      trimmed_atom_data[i].y = atom_data[i].y;
      trimmed_atom_data[i].z = atom_data[i].z;
   }

   return trimmed_atom_data;
}