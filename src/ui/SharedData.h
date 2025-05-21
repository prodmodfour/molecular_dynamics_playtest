#pragma once
#include <vector>
#include "../simulation/Timestep.h"
#include "../simulation/Config.h"
#include "../atoms/Atom.h"
#include "../atoms/AtomPairLibrary.h"
#include <mutex>
#include <iostream>
#include <algorithm>
#include <string>




typedef struct SharedData
{
    SharedData()
    {
        indexes_of_timesteps_edited_by_ui = std::vector<int>();
        buffer_size = 1000;
        index_of_latest_timestep_simulated = 0;
        index_of_latest_timestep_displayed = 0;
        changed_by_ui_since_last_loop = false;
        config = simulation::Config();
        time = 0.0;
        atom_pair_library = atoms::AtomPairLibrary();
        simulation_ended = false;
    };
    std::vector<int> indexes_of_timesteps_edited_by_ui;
    int buffer_size;
    int index_of_latest_timestep_simulated;
    int index_of_latest_timestep_displayed;
    bool changed_by_ui_since_last_loop;
    simulation::Config config;
    double time;
    atoms::AtomPairLibrary atom_pair_library;
    std::mutex mutex;
    bool simulation_ended;

} SharedData;




