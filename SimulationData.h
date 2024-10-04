#ifndef __AtomSystemData_h
#define __AtomSystemData_h
#include <vector>
#include <Atom.h>

typedef struct 
{
    double total;
    double kinetic;
    double potential;
} Energy;

class State:
{
    public:
        
    private:
        std::vector<Atom> atom_data;
        double temperature;
        Energy energy; 
    
};

class SimulationData:
{
    public:
    private:
        int current_state_index;
        int total_states;

        State original_state;
        std::vector<State> states;


    


};



#endif