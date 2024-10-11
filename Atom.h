#ifndef __Atom_h
#define __Atom_h

#include <vector>


typedef struct {
   std::string element;
   double x, y, z;
   double vx, vy, vz;
   double fx, fy, fz;
   double ke;
   double reference_ke;
   double m;
   bool hidden;
} Atom;




#endif