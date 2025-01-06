#pragma once
#include <string>
#include "IDAssigner.h"

class Atom
{
    public:
        int id;
        double x, y, z;
        double fx, fy, fz;
        double vx, vy, vz;
        double mass;
        double radius;
        double kinetic_energy;
        std::string type;
        std::string name;

        // Default Constructor
        Atom(std::string type, double mass, double radius, IDAssigner &id_assigner)
        {
            x = 0;
            y = 0;
            z = 0;
            fx = 0;
            fy = 0;
            fz = 0;
            vx = 0;
            vy = 0;
            vz = 0;
            this->mass = mass;
            this->radius = radius;
            kinetic_energy = 0;
            this->type = type;
            id = id_assigner.get_new_id();
            name = "atom";
        }
};