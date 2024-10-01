#ifndef __Type_atom_h
#define __Type_atom_h

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

#endif