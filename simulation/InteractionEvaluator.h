#pragma once
#include "AtomPairLibrary.h"
#include "Atom.h"
#include <vector>
#include "Config.h"
#include "TotalEnergy.h"
#include <cmath>
#include "AtomPairData.h"

class InteractionEvaluator
{
    public:
        InteractionEvaluator(AtomPairLibrary &atom_pair_library, std::vector<Atom> &atoms)
        {
            this->atom_pair_library = atom_pair_library;
            this->atoms = atoms;

        }

        AtomPairLibrary atom_pair_library;
        std::vector<Atom> atoms;



        void evaluate_interactions(Config config, TotalEnergy &total_energy)
        {
            zero_forces();

            double xi, yi, zi;
            double xj, yj, zj;
            double dx, dy, dz;
            double fxij, fyij, fzij, fij;
            double squared_distance, distance, sigma_squared;
            double sr2, sr6, sr12, epsilon4, epsilon24;
            double potential_ij, potential_energy, unit_vector_x, unit_vector_y, unit_vector_z;
            double reciprocal_distance;


            total_energy.potential = 0;

            double r_cutoff = config.r_cutoff;
            double r_cutoff_squared = r_cutoff * r_cutoff;

            for (int i = 0; i < atoms.size(); i++)
            {
                Atom &atom_i = atoms[i];
                xi = atom_i.x;
                yi = atom_i.y;
                zi = atom_i.z;

                for (int j = i + 1; j < atoms.size(); j++)
                {
                    Atom &atom_j = atoms[j];
                    xj = atom_j.x;
                    yj = atom_j.y;
                    zj = atom_j.z;

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

                    AtomPairData atom_pair_data = atom_pair_library.get_atom_pair_data(atom_i.type, atom_j.type);
                    double sigma = atom_pair_data.sigma;
                    double epsilon = atom_pair_data.epsilon;

                    sigma_squared = sigma * sigma;
                    sr2 = sigma_squared / squared_distance;
                    sr6 = sr2 * sr2 * sr2;
                    sr12 = sr6 * sr6;
                    epsilon4 = 4 * epsilon;
                    epsilon24 = 24 * epsilon;

                    reciprocal_distance = 1 / distance;
                    unit_vector_x = dx * reciprocal_distance;
                    unit_vector_y = dy * reciprocal_distance;
                    unit_vector_z = dz * reciprocal_distance;

                    potential_ij = epsilon4 * (sr12 - sr6);
                    total_energy.potential += potential_ij;

                    
                    fij = epsilon24 * (2 * sr12 - sr6) * reciprocal_distance;
                    fxij = fij * unit_vector_x;
                    fyij = fij * unit_vector_y;
                    fzij = fij * unit_vector_z; 

                    /* Sum the forces on i due to all j */
                    atoms[i].fx += fxij;
                    atoms[i].fy += fyij;
                    atoms[i].fz += fzij;

                    /* Sum the forces on j due to i in ij direction */
                    atoms[j].fx -= fxij;
                    atoms[j].fy -= fyij;
                    atoms[j].fz -= fzij;  



                }

                // Convert to eV
                total_energy.potential *= 1.602176634e-19;
            }
        }

        void zero_forces()
        {
            for (Atom &atom : atoms)
            {
                atom.fx = 0.0;
                atom.fy = 0.0;
                atom.fz = 0.0;
            }
        }
}

