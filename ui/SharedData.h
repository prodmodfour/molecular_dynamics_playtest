#pragma once
#include <vector>
#include "../simulation/Timestep.h"
#include "../simulation/Config.h"
#include "../atoms/Atom.h"
#include "../atoms/AtomPairLibrary.h"
#include <mutex>



class SharedData
{
    public:
        SharedData();
        std::vector<int> indexes_of_timesteps_edited_by_ui;
        int buffer_size;
        int index_of_latest_timestep_simulated;
        int index_of_latest_timestep_displayed;
        bool changed_by_ui_since_last_loop;
        simulation::Config config;
        double time;
        atoms::AtomPairLibrary atom_pair_library;
        std::mutex mutex;
    
}

class LocalUnsavedChangesToSharedData : public SharedData
{
    public:
        LocalUnsavedChangesToSharedData();
        
        // bools to signify whether a variable in the parent class has been changed since the last loop.
        bool simulation_data_changed_since_last_loop;
        bool config_changed_since_last_loop;
        bool time_changed_since_last_loop;
        bool atom_pair_library_changed_since_last_loop;
        bool latest_timestep_index_changed_since_last_loop;
        bool buffer_size_changed_since_last_loop;
        bool changed_by_ui_since_last_loop;



        
        
}
