#ifndef __SimulationConfig_h
#define __SimulationConfig_h

class SimulationConfig
{
    public:
        // Constructor declaration with default parameters
        SimulationConfig(
            double atom_radius_ = 1.28,
            double atom_mass_ = 63.546,
            int cubes_in_x_ = 8,
            int cubes_in_y_ = 4,
            int cubes_in_z_ = 4,
            double atom_spacing_ = 3.61,
            double energy_applied_to_impact_atom_ = 10,
            double impact_atom_x_offset_ = 0,
            double impact_atom_y_offset_ = 6,
            double impact_atom_z_offset_ = 0,
            double scaling_ = 0.01,
            double epsilon_ = 0,
            double sigma_ = 2.285,
            double rcutoff_ = 0,
            double velocity_scale_ = 0,
            double simulation_timestep_size_ = 0.001,
            bool add_impact_on_ = true,
            bool parallel_projection_on_ = false,
            double impact_atom_x_energy_share_ = 0.0,
            double impact_atom_y_energy_share_ = 1.0,
            double impact_atom_z_energy_share_ = 0.0
        ) : 
            atom_radius(atom_radius_),
            atom_mass(atom_mass_),
            cubes_in_x(cubes_in_x_),
            cubes_in_y(cubes_in_y_),
            cubes_in_z(cubes_in_z_),
            atom_spacing(atom_spacing_),
            energy_applied_to_impact_atom(energy_applied_to_impact_atom_),
            impact_atom_x_offset(impact_atom_x_offset_),
            impact_atom_y_offset(impact_atom_y_offset_),
            impact_atom_z_offset(impact_atom_z_offset_),
            scaling(scaling_),
            epsilon(epsilon_),
            sigma(sigma_),
            rcutoff(rcutoff_),
            velocity_scale(velocity_scale_),
            simulation_timestep_size(simulation_timestep_size_),
            add_impact_on(add_impact_on_),
            parallel_projection_on(parallel_projection_on_),
            impact_atom_x_energy_share(impact_atom_x_energy_share_),
            impact_atom_y_energy_share(impact_atom_y_energy_share_),
            impact_atom_z_energy_share(impact_atom_z_energy_share_)
        {
            // Calculate derived variables
            epsilon = 0.4802 * ev_to_j_per_mole;
            rcutoff = 2.5 * sigma;
            velocity_scale = scaling * simulation_timestep_size / atom_mass;
        }

        // Atom properties
        double atom_radius;

        
        // Simulation grid configuration
        int cubes_in_x;
        int cubes_in_y;
        int cubes_in_z;
        double atom_spacing;
        
        // Impact atom configuration
        double energy_applied_to_impact_atom;
        double impact_atom_x_offset;
        double impact_atom_y_offset;
        double impact_atom_z_offset;
        
        // Energy conversion factors
        const double ev_to_j_per_mole = 96400.0;
        const double j_per_mole_to_ev = 1.037e-5;

        // Simulation values
        double sigma;
        double rcutoff;
        
        // Getters and setters for moved variables
        double getAtomMass() const { return atom_mass; }
        void setAtomMass(double value) { 
            atom_mass = value;
            updateVelocityScale();
        }

        double getScaling() const { return scaling; }
        void setScaling(double value) { 
            scaling = value;
            updateVelocityScale();
        }

        double getEpsilon() const { return epsilon; }
        void setEpsilon(double value) { epsilon = value; }

        double getSigma() const { return sigma; }
        void setSigma(double value) { sigma = value; }

        double getRcutoff() const { return rcutoff; }
        void setRcutoff(double value) { rcutoff = value; }

        double getVelocityScale() const { return velocity_scale; }
        
        double getSimulationTimestepSize() const { return simulation_timestep_size; }
        void setSimulationTimestepSize(double value) { 
            simulation_timestep_size = value;
            updateVelocityScale();
        }

        // Visualization and impact settings
        bool add_impact_on;
        bool parallel_projection_on;
        double impact_atom_x_energy_share;
        double impact_atom_y_energy_share;
        double impact_atom_z_energy_share;

    private:
        // Moved from public to private
        double atom_mass;
        double scaling;
        double epsilon;
        double velocity_scale;
        double simulation_timestep_size;

        // Helper method to update velocity_scale when dependencies change
        void updateVelocityScale() {
            velocity_scale = scaling * simulation_timestep_size / atom_mass;
        }
};

#endif