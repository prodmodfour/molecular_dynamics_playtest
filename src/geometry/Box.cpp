#include "Box.h"

namespace geometry {

Box::Box()
{
}

Box::Box(std::vector<atoms::Atom> &atoms)
{
    this->corners = determine_cuboid_corners(atoms);
            
    this->top_plane    = Plane(corners[1], corners[3], corners[7], corners[5]);
    this->bottom_plane = Plane(corners[0], corners[4], corners[6], corners[2]);
    this->left_plane   = Plane(corners[0], corners[2], corners[3], corners[1]);
    this->right_plane  = Plane(corners[4], corners[5], corners[7], corners[6]);
    this->front_plane  = Plane(corners[0], corners[1], corners[5], corners[4]);
    this->back_plane   = Plane(corners[2], corners[6], corners[7], corners[3]);
}

}