#ifndef __Settings_h
#define __Settings_h
#include "Type_atom.h"
#include "file_functions.h"
#include "string_functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

class parameter
{
    private:
        std::string name;
        std::string value;
        std::string data_type;

    public:
        parameter(std::string name)
        {

        }

        parameter(std::string name, int value)
        {
            this->value = std::to_string(value);
            this->data_type = "int";
        }

        parameter(std::string name, double value)
        {
            this->value = std::to_string(value);
            this->data_type = "double";
        }

        
        parameter(std::string name, std::string value)
        {
            this->value = value;
            this->data_type = "std::string";
        }

        void operator=(int value)
        {
            this->value = std::to_string(value);
            this->data_type = "int";
        }
        
        void operator=(double value)
        {
            this->value = std::to_string(value);
            this->data_type = "double";
        }
        
        void operator=(std::string value)
        {
            this->value = value;
            this->data_type = "std::string";
        }

        void print()
        {
            std::cout << value << std::endl;
        }

};
typedef std::map<std::string, parameter> parameter_map;
typedef std::pair<std::string, parameter> string_parameter_pair;

class Settings
{
    private:
        int cubes_in_x;
        int cubes_in_y; 
        int cubes_in_z;
        int animation_step_duration;
        double simulation_timestep_size;
        double simulation_history_interval;
        int simulation_total_timesteps;

        double atom_spacing;
        Type_atom impact_atom_offsets;
        double energy_applied_to_impact_atom;

        double atom_mass;
        double atom_radius;

        double epsilon;
        double sigma;
        double r_cutoff;

        std::string mode; // This can be file or generate
        std::string atoms_filename; // This would be a file that contains xyz coordinates of an atom system to be read

        parameter_map parameter_map; 
        double ev_to_j_per_mole;
        double j_per_mole_to_ev;
        double velocity_scale;
        double scaling;


    public:
        Settings(std::vector<std::string> arguments)
        {

        }



        // Getters and Setters
        int GetCubesInX() const { return cubes_in_x; }
        void SetCubesInX(int CubesInX) { cubes_in_x = CubesInX; }

        int GetCubesInY() const { return cubes_in_y; }
        void SetCubesInY(int CubesInY) { cubes_in_y = CubesInY; }

        int GetCubesInZ() const { return cubes_in_z; }
        void SetCubesInZ(int CubesInZ) { cubes_in_z = CubesInZ; }

        int GetAnimationStepDuration() const { return animation_step_duration; }
        void SetAnimationStepDuration(int AnimationStepDuration) { animation_step_duration = AnimationStepDuration; }

        double GetSimulationTimestepSize() const { return simulation_timestep_size; }
        void SetSimulationTimestepSize(double SimulationTimestepSize) { simulation_timestep_size = SimulationTimestepSize; }

        int GetSimulationTotalTimesteps() const { return simulation_total_timesteps; }
        void SetSimulationTotalTimesteps(int TotalTimesteps) { simulation_total_timesteps = TotalTimesteps; }

        double GetSimulationHistoryInterval() const { return simulation_history_interval; }
        void SetSimulationHistoryInterval(double SimulationHistoryInterval) { simulation_history_interval = SimulationHistoryInterval; }

        double GetAtomSpacing() const { return atom_spacing; }
        void SetAtomSpacing(double AtomSpacing) { atom_spacing = AtomSpacing; }

        double GetEnergyAppliedToImpactAtom() const { return energy_applied_to_impact_atom; }
        void SetEnergyAppliedToImpactAtom(double EnergyAppliedToImpactAtom) { energy_applied_to_impact_atom = EnergyAppliedToImpactAtom; }

        double GetAtomMass() const { return atom_mass; }
        void SetAtomMass(double AtomMass) { atom_mass = AtomMass; }

        double GetAtomRadius() const { return atom_radius; }
        void SetAtomRadius(double AtomRadius) { atom_radius = AtomRadius; }

        double GetEVToJPerMole() const { return ev_to_j_per_mole; }

        double GetJPerMoleToEV() const { return j_per_mole_to_ev; }

        double GetVelocityScale() const { return velocity_scale; }
        void SetVelocityScale(double VelocityScale) { velocity_scale = VelocityScale; }

        double GetEpsilon() const { return epsilon; }
        void SetEpsilon(double Epsilon) { epsilon = Epsilon; }

        double GetSigma() const { return sigma; }
        void SetSigma(double Sigma) { sigma = Sigma; }

        double GetRCutoff() const { return r_cutoff; }
        void SetRCutoff(double RCutoff) { r_cutoff = RCutoff; }
        
        Type_atom ImpactAtomOffsets() const { return impact_atom_offsets; }
        void SetImpactAtomOffsets(double x_offset, double y_offset, double z_offset) 
        { 
            impact_atom_offsets.x = x_offset;
            impact_atom_offsets.y = y_offset;
            impact_atom_offsets.z = y_offset;
        }


        // Printers
        void PrintSettings()
        {

        }

        void PrintHelpMessage()
        {
            std::cout << "Console Flags" << std::endl;
            std::cout << "-help Show help message" << std::endl;

        }






};

#endif