#pragma once
#include <vector>
#include "Atom.h"
#include <tuple>
#include <vector>
#include "Config.h"




class AlltoAllAtomPairer
{
    public:
        std::vector<Atom> atoms;
        
        AlltoAllAtomPairer(std::vector<Atom> &atoms)
        {
            this->atoms = atoms;
        }

        std::vector<std::tuple<int, int, double, double>> pair_atoms(Config config)
        {
            std::vector<std::tuple<int, int, double, double>> atom_pairs;

            double r_cutoff = config.r_cutoff;
            double r_cutoff_squared = r_cutoff * r_cutoff;
            double xi, yi, zi;
            double xj, yj, zj;
            double dx, dy, dz;
            double squared_distance, distance;

            for (int i = 0; i < atoms.size(); i++)
            {
                xi = atoms[i].x;
                yi = atoms[i].y;
                zi = atoms[i].z;
                
                for (int j = 0; j < atoms.size(); j++)
                {
                    xj = atoms[j].x;
                    yj = atoms[j].y;
                    zj = atoms[j].z;

                    dx = xj - xi;
                    if (dx > r_cutoff)
                    {
                        continue;
                    }
                    dy = yj - yi;
                    if (dy > r_cutoff)
                    {
                        continue;
                    }
                    dz = zj - zi;
                    if (dz > r_cutoff)
                    {
                        continue;
                    }

                    squared_distance = dx * dx + dy * dy + dz * dz;
                    if (squared_distance > r_cutoff_squared)
                    {
                        continue;
                    }
                    distance = sqrt(squared_distance);
                    atom_pairs.push_back(std::make_tuple(i, j, distance, 0));
                }
            }

            return atom_pairs;
        }

};

