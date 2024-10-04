#ifndef __Atom_h
#define __Atom_h

#include <vector>


typedef struct {
   double x, y, z;
   double vx, vy, vz;
   double fx, fy, fz;
   double kinetic_energy;
   double mass;
} Atom;




#endif