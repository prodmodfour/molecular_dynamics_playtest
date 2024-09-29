#ifndef __Settings_h
#define __Settings_h
#include "Type_atom.h"
#include "file_functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>


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
        double impact_atom_y_offset;
        double energy_applied_to_impact_atom;

        double atom_mass;
        double atom_radius;

        double ev_to_j_per_mole;
        double j_per_mole_to_ev;

        double velocity_scale;
        double scaling;

        double epsilon;
        double sigma;
        double r_cutoff;

        std::string mode; // This can be file or generate

        std::map <std::string, int> setting_hash_map;


        void _InitialiseDefaults()
        {
            cubes_in_x = 4;
            cubes_in_y = 4;

        }
        
        void _UpdateSettingsFromFile(std::ifstream settings_file)
        {
            std::string line;
            std::vector<std::string> words;
            int setting_hash;
            while (getline(settings_file, line))
            {
                words = split_sentence(line);

                try
                {
                    setting_hash = setting_hash_map.at(words[0]);
                }
                catch (const std::out_of_range)
                {
                    setting_hash = 0;
                }

                switch (setting_hash)
                {

                }
            }
        }

        void _SaveToFile()
        {
            std::ofstream settings_file;
            settings_file.open("settings.ini");

            // settings_file << "cubes_in_x = 4\n";
            settings_file << "cubes_in_x = 4" << std::endl;
            settings_file << "cubes_in_y = 4" << std::endl;
            settings_file.close();
        }


    public:
        Settings(std::vector<std::string> arguments)
        {
            _InitialiseDefaults();
            std::ifstream settings_file;
            settings_file.open("settings.ini");
            if (settings_file)
            {
                _UpdateSettingsFromFile(settings_file);
            }
            settings_file.close();

            _SaveToFile();

            
            


        }

        Settings(int argc, char *argv[])
        {
            if (argc == 1)
            {
                cubes_in_x = 2;
                cubes_in_z = 2;
                cubes_in_y = 2;
                animation_step_duration = 50;
                simulation_timestep_size = 0.0001;
                simulation_total_timesteps = 1000;
                energy_applied_to_impact_atom = 1000;
            }
            else
            {
                cubes_in_x = std::stoi(argv[1]);
                cubes_in_y = std::stoi(argv[2]);
                cubes_in_z = std::stoi(argv[3]);
                animation_step_duration = std::stoi(argv[4]);
                simulation_timestep_size = std::stod(argv[5]);
                simulation_total_timesteps = std::stoi(argv[6]);
                energy_applied_to_impact_atom = std::stod(argv[7]);
            }
            
            atom_spacing = 3.61;
            impact_atom_y_offset = 3;
            

            atom_mass = 63.546;
            atom_radius = 1.28;

            ev_to_j_per_mole = 96400.0;
            j_per_mole_to_ev = 1.037e-5;

            scaling = 0.01;

            epsilon = 0.4802 * ev_to_j_per_mole;
            sigma = 2.285;
            r_cutoff = 2.5 * sigma;
            velocity_scale = scaling * simulation_timestep_size / atom_mass;

            simulation_history_interval = 50;

        }

        Settings(std::vector<Type_atom> all_atoms, int argc, char *argv[])
        {


            animation_step_duration = std::stoi(argv[3]);
            simulation_timestep_size = std::stod(argv[4]);
            simulation_total_timesteps = std::stoi(argv[5]);
            energy_applied_to_impact_atom = std::stod(argv[6]);

            
            impact_atom_y_offset = 3;
            

            atom_mass = 63.546;
            atom_radius = 1.28;

            ev_to_j_per_mole = 96400.0;
            j_per_mole_to_ev = 1.037e-5;

            scaling = 0.01;

            epsilon = 0.4802 * ev_to_j_per_mole;
            sigma = 2.285;
            r_cutoff = 2.5 * sigma;
            velocity_scale = scaling * simulation_timestep_size / atom_mass;

            simulation_history_interval = 50;

        }

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

        double GetImpactAtomYOffset() const { return impact_atom_y_offset; }
        void SetImpactAtomYOffset(double ImpactAtomYOffset) { impact_atom_y_offset = ImpactAtomYOffset; }

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

        // Printers
        void PrintSettings()
        {

        }

        void PrintHelpMessage()
        {
            std::cout << "Console Flags" << std::endl;
            std::cout << "-h Show help message" << std::endl;

        }


};

#endif