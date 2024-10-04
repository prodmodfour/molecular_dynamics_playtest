#ifndef __SimulationData_h
#define __SimulationData_h
#include <vector>
#include <Atom.h>
#include "Settings.h"

#include <iostream>
#include <vector>

class Energy {
public:
    Energy() : kinetic(0.0), potential(0.0), total(0.0) {}

    Energy(double kineticEnergy, double potentialEnergy)
        : kinetic(kineticEnergy), potential(potentialEnergy), total(kineticEnergy + potentialEnergy) {}

    double get_total() const {
        return total;
    }

    double get_kinetic() const {
        return kinetic;
    }

    double get_potential() const {
        return potential;
    }

private:
    double total;
    double kinetic;
    double potential;
};

class Snapshot {
    public:
        Snapshot(std::vector<Atom> atoms, Settings settings)
        {
            this->settings = settings;
            this->atoms = atoms;
            double potential_energy = calculate_potential_energy();

            double kinetic_energy = sum_kinetic_energies();
            Energy energy(kinetic_energy, potential_energy);
            this->energy = energy;
            this->temperature = calculate_temperature();
        }

        Snapshot(std::vector<Atom> atoms, double potential_energy, Settings settings)
        {
            this->settings = settings;
            this->atoms = atoms;
            double kinetic_energy = sum_kinetic_energies();
            Energy energy(kinetic_energy, potential_energy);
            this->energy = energy;
            this->temperature = calculate_temperature();
        }

        Snapshot(std::vector<Atom> atoms, double kinetic_energy, double potential_energy, Settings settings)
        {
            this->settings = settings;
            this->atoms = atoms;
            Energy energy(kinetic_energy, potential_energy);
            this->energy = energy;
            this->temperature = calculate_temperature();
        }

        std::vector<Atom> get_atoms() const {
            return atoms;
        }

        double get_temperature() const {
            return temperature;
        }

    private:
        std::vector<Atom> atoms;
        double temperature; // In Kelvin
        Energy energy;
        Settings settings;

        double sum_kinetic_energies() {
            // Implementation for summing kinetic energies
        }

        double calculate_potential_energy() {
            // Implementation for calculating potential energy
        }

        double calculate_temperature() {
            // Implementation for calculating temperature
        }
};

class Simulation {
public:
    SimulationData(Snapshot original_snapshot, Settings& settings)
        : original_snapshot(original_snapshot), current_snapshot(original_snapshot), settings(settings) 
    {
        this->settings = settings;
        this->current_time = 0.0;
        this->current_snapshot_index = 0;
        this->total_snapshots = 1;
    }

    void advance_to_next_snapshot() {
        // Logic to advance to the next snapshot in the simulation
    }

private:
    double current_time;
    int current_snapshot_index;
    int total_snapshots;

    const Snapshot original_snapshot;
    Snapshot current_snapshot;
    std::vector<Snapshot> snapshots;
    Settings& settings;
};

#endif