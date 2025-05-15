#include "Atom.h"

namespace atoms {

Atom::Atom(std::string type, double mass, double radius)
    : x(0.0), y(0.0), z(0.0),
      fx(0.0), fy(0.0), fz(0.0),
      vx(0.0), vy(0.0), vz(0.0),
      mass(mass),
      radius(radius),
      kinetic_energy(0.0),
      type(type),
      name("atom")
{
}

geometry::Vector Atom::get_position()
{
    return geometry::Vector(x, y, z);
}

}