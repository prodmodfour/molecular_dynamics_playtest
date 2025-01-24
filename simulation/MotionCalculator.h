#pragma once
#include "Atom.h"
#include <vector>
#include "Config.h"


class MotionCalculator
{
    public:
        MotionCalculator(std::vector<Atom> &atoms)
        {
            this->atoms = atoms;

        }

        std::vector<Atom> atoms;


        void calculate_motion(Config config, Atom &atom)
        {
            double v_squared;
            double vxi, vyi, vzi;
            double vxi2, vyi2, vzi2;
            double vxi3, vyi3, vzi3;
            double fxi, fyi, fzi;
            double delta_vxi, delta_vyi, delta_vzi;
            double scaling = 0.01;
            
            for (Atom &atom : atoms)
            {
                double velocity_scale = scaling * config.timestep_size / atom.mass;

                // Calculate velocity V(t + 0.5dt)
                vxi = atom.vx;
                vyi = atom.vy;
                vzi = atom.vz;

                fxi = atom.fx;
                fyi = atom.fy;
                fzi = atom.fz;

                delta_vxi = fxi * velocity_scale;
                delta_vyi = fyi * velocity_scale;
                delta_vzi = fzi * velocity_scale;

                vxi2 = vxi + delta_vxi;
                vyi2 = vyi + delta_vyi;
                vzi2 = vzi + delta_vzi;

                // Update positions
                atom.x += vxi2 * config.timestep_size;
                atom.y += vyi2 * config.timestep_size;
                atom.z += vzi2 * config.timestep_size;

                // Calculate actual velocity at time t
                // For kinetic energy calculations only
                vxi3 = (vxi + vxi2) / 2;
                vyi3 = (vyi + vyi2) / 2;
                vzi3 = (vzi + vzi2) / 2;
                v_squared = vxi3*vxi3 + vyi3*vyi3 + vzi3*vzi3;
                atom.kinetic_energy = 0.5 * atom.mass * v_squared;
                // Convert to eV
                atom.kinetic_energy *= 1.602176634e-19;



                // Update velocities
                atom.vx = vxi2;
                atom.vy = vyi2;
                atom.vz = vzi2;
            }
        }

        
}
