#ifndef __simulation_h
#define __simulation_h

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <chrono>
#include <thread>
#include <cstdlib>
// #include <vtkTextActor.h>
// #include <vtkTextProperty.h>

#include "Settings.h"
#include "Atom.h"
#include "SimulationData.h"

void zero_forces(std::vector<Atom> &all_atoms);
double evaluate_forces(std::vector<Atom> &all_atoms, Settings settings);
double calculate_kinetic_energy(double sum_v_squared, Settings settings);

Frame create_next_frame(Frame frame, Settings settings)
{

    std::vector<Atom> all_atoms = frame.all_atoms;
    double time = frame.time;

    auto start = std::chrono::high_resolution_clock::now();
 
    double potential_energy, v_squared, total_kinetic_energy, average_ke;
    double vxi, vyi, vzi;
    double vxi2, vyi2, vzi2;
    double vxi3, vyi3, vzi3;
    double fxi, fyi, fzi;
    double delta_vxi, delta_vyi, delta_vzi;
    int total_timesteps = 1;
    double velocity_scale = settings.get_velocity_scale();
    double timestep_size = settings.get_simulation_timestep_size();

    // Leapfrog Verlet Algorithm
    for (int timestep = 0; timestep < total_timesteps; timestep++)
    {
        // Reset variables
        zero_forces(all_atoms);

        total_kinetic_energy = 0.0;


        potential_energy = evaluate_forces(all_atoms, settings);

        // Simulate Atom Trajectory
        for (int i = 0; i < all_atoms.size(); i++)
        {
            // Calculate velocity V(t + 0.5dt)
            vxi = all_atoms[i].vx;
            vyi = all_atoms[i].vy;
            vzi = all_atoms[i].vz;

            fxi = all_atoms[i].fx;
            fyi = all_atoms[i].fy;
            fzi = all_atoms[i].fz;

            delta_vxi = fxi * velocity_scale;
            delta_vyi = fyi * velocity_scale;
            delta_vzi = fzi * velocity_scale;

            vxi2 = vxi + delta_vxi;
            vyi2 = vyi + delta_vyi;
            vzi2 = vzi + delta_vzi;

            // Update positions
            all_atoms[i].x += vxi2 * timestep_size;
            all_atoms[i].y += vyi2 * timestep_size;
            all_atoms[i].z += vzi2 * timestep_size;

            // Calculate actual velocity at time t
            // For kinetic energy calculations only
            vxi3 = (vxi + vxi2) / 2;
            vyi3 = (vyi + vyi2) / 2;
            vzi3 = (vzi + vzi2) / 2;
            v_squared = vxi3*vxi3 + vyi3*vyi3 + vzi3*vzi3;
            all_atoms[i].ke = calculate_kinetic_energy(v_squared, settings);
            total_kinetic_energy += all_atoms[i].ke;



            // Update velocities
            all_atoms[i].vx = vxi2;
            all_atoms[i].vy = vyi2;
            all_atoms[i].vz = vzi2;

            
    }
        average_ke = total_kinetic_energy / all_atoms.size();
        time += timestep_size;
        // std::ostringstream oss;
        // // Set time to 3 decimal place
        // oss << std::fixed << std::setprecision(3) << time;
        // std::string time_string = oss.str();

        // std::ostringstream oss1;

        // // Set energies to 4 sig fig
        // double te = (total_kinetic_energy + potential_energy);
        // oss1 << std::setprecision(4) << te;
        // std::string te_string = oss1.str();
        // oss1.str("");
        // oss1.clear();
        // oss1 << std::setprecision(4) << (total_kinetic_energy);
        // std::string ke_string = oss1.str();
        // oss1.str("");
        // oss1.clear();
        // oss1 << std::setprecision(4) << (potential_energy);
        // std::string pe_string = oss1.str();
        // oss1.str("");
        // oss1.clear();
        // oss1 << std::setprecision(4) << (average_ke);
        // std::string ake_string = oss1.str();

        // std::string reading = "Time: " + time_string + " ps " " TE: " + te_string + " eV " + " KE: " + ke_string + " eV "  + " PE: "  + pe_string + " eV" + " Average ke: " + ake_string + " eV";

        
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = end - start;
    // std::cout << duration.count() << "s " << std::endl;

    Frame new_frame(all_atoms, total_kinetic_energy, potential_energy, total_kinetic_energy + potential_energy, time);


    return new_frame;
}



void zero_forces(std::vector<Atom> &all_atoms)
{
    for (int i = 0; i < all_atoms.size(); i++)
    {
        all_atoms[i].fx = 0;
        all_atoms[i].fy = 0;
        all_atoms[i].fz = 0;
    }
}

double evaluate_forces(std::vector<Atom> &all_atoms, Settings settings)
{
 double xi, yi, zi;
 double xj, yj, zj;
 double dx, dy, dz;
 double fxij, fyij, fzij, fij;
 double squared_distance, distance, sigma_squared;
 double sr2, sr6, sr12, epsilon4, epsilon24;
 double potential_ij, potential_energy, unit_vector_x, unit_vector_y, unit_vector_z;
 double reciprocal_distance;

 potential_energy = 0;


 double epsilon = settings.get_epsilon();
 double sigma = settings.get_sigma();
 // We will only ever have to deal with Cu-Cu interactions
 epsilon4 = 4 * epsilon;
 epsilon24 = 24 * epsilon;
 sigma_squared = sigma * sigma;

double r_cutoff = settings.get_rcutoff();
double r_cutoff_squared = r_cutoff * r_cutoff;
 // Find forces from every unique pair interaction
 for (int i = 0; i < all_atoms.size(); i++)
 {

    xi = all_atoms[i].x;
    yi = all_atoms[i].y;
    zi = all_atoms[i].z;

    for (int j = i + 1; j < all_atoms.size(); j++)
    {
        xj = all_atoms[j].x;
        yj = all_atoms[j].y;
        zj = all_atoms[j].z;

        dx = xi - xj;
        if (dx > r_cutoff)
        {
            continue;
        }
        dy = yi - yj;
        if (dy > r_cutoff)
        {
            continue;
        }
        dz = zi - zj;
        if (dz > r_cutoff)
        {
            continue;
        }

        squared_distance = dx*dx + dy*dy + dz*dz;
        if (squared_distance > r_cutoff_squared)
        {
            continue;
        }

        distance = std::sqrt(squared_distance);

        // Calculate force and potential between i and j
        sr2 = sigma_squared / squared_distance;
        sr6 = sr2 * sr2 * sr2;
        sr12 = sr6 * sr6;

        reciprocal_distance = 1 / distance;
        unit_vector_x = dx * reciprocal_distance;
        unit_vector_y = dy * reciprocal_distance;
        unit_vector_z = dz * reciprocal_distance;

        potential_ij = sr12 - sr6;
        potential_energy += potential_ij;

        fij = (2 * sr12 - sr6) * reciprocal_distance;
        fxij = fij * unit_vector_x;
        fyij = fij * unit_vector_y;
        fzij = fij * unit_vector_z; 

        /* Sum the forces on i due to all j */
        all_atoms[i].fx += fxij;
        all_atoms[i].fy += fyij;
        all_atoms[i].fz += fzij;

        /* Sum the forces on j due to i in ij direction */
        all_atoms[j].fx -= fxij;
        all_atoms[j].fy -= fyij;
        all_atoms[j].fz -= fzij;          
    }


    
    

 }
    for (int i = 0; i < all_atoms.size(); i++)
    {
        all_atoms[i].fx *= epsilon24;
        all_atoms[i].fy *= epsilon24;
        all_atoms[i].fz *= epsilon24;
    }
     // Multiply by energy factor
    potential_energy *= epsilon4;

    // Convert to eV
    double j_per_mole_to_ev = settings.get_j_per_mole_to_ev();
    potential_energy *= j_per_mole_to_ev;

    return potential_energy;

}

double calculate_kinetic_energy(double sum_v_squared, Settings settings)
{
    double kinetic_energy;
    double atom_mass = settings.get_atom_mass();
    kinetic_energy = 0.5*atom_mass*sum_v_squared;
    
    // Convert to eV
    double j_per_mole_to_ev = settings.get_j_per_mole_to_ev();
    kinetic_energy *= j_per_mole_to_ev;
    return kinetic_energy;
}



#endif