#ifndef __AtomSystemData_h
#define __AtomSystemData_h
#include <vector>
#include <Atom.h>

class Energy 
{
    public:
        Energy(double kineticEnergy, double potentialEnergy)
            : kinetic(kineticEnergy), potential(potentialEnergy), total(kineticEnergy + potentialEnergy) {}

        double getTotal() const {
            return total;
        }

        double getKinetic() const {
            return kinetic;
        }

        double getPotential() const {
            return potential;
        }

    private:
        double total;
        double kinetic;
        double potential;
};

class State
{
    public:
    State(std::vector<Atom> atoms, double potential_energy)
    {
        this-> atoms = atoms;
        double kinetic_energy = sum_kinetic_energies();
        Energy energy(kinetic_energy, potential_energy);
        this->energy = energy;
        this->temperature = calculate_temperature();

    }

    State(std::vector<Atom> atoms, double kinetic_energy, double potential_energy)
    {
        this-> atoms = atoms;
        Energy energy(kinetic_energy, potential_energy);
        this->energy = energy;
        this->temperature = calculate_temperature();
    }
    
    std::vector<Atom> get_atoms()
    {
        return atoms;
    }

    double get_temperature()
    {
        return temperature;
    }


    private:
        std::vector<Atom> atoms;
        double temperature; // In Kelvin
        Energy energy;
    
        double sum_kinetic_energies()
        {

        }

        double calculate_temperature()
        {

        }
    
};

class SimulationData
{
    public:

    private:
        int current_state_index;
        int total_states;

        const State original_state;
        std::vector<State> states;


    


};



#endif