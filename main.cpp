#include "atoms/Atom.h"
#include "atoms/Structure.h"
#include "atoms/atom_generation_functions.h"

#include "simulation/simulation_data_mutex.h"
#include "simulation/simulation_functions.h"
#include "simulation/Config.h"
#include "simulation/Timestep.h"

#include "geometry/Box.h"
#include "geometry/geometry_functions.h"
#include "geometry/Vector.h"

#include "ui/data_loaders/BasicDataLoader.h"
#include "ui/PlaybackSettings.h"
#include "ui/main_visualiser_widgets/MDVisualiser.h"
#include "ui/SharedData.h"

#include "physics/physics_functions.h"



#include <vector>
#include <iostream>
#include <QApplication>
#include <QSettings>
#include <thread>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSettings developer_settings(
            QSettings::IniFormat,
            QSettings::UserScope,
            "STFC",
            "developer_settings");

    if (!developer_settings.contains("first_run") || developer_settings.value("first_run").toBool())
    {
        developer_settings.setValue("first_run", false);
        developer_settings.sync();
    }
    else
    {
        developer_settings.sync();
    }
    
    return 0;
}