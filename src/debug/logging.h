#pragma once

#include <iostream>


#include <QLoggingCategory>


Q_DECLARE_LOGGING_CATEGORY(ui_logging_category)
Q_DECLARE_LOGGING_CATEGORY(simulation_logging_category)
Q_DECLARE_LOGGING_CATEGORY(geometry_logging_category)
Q_DECLARE_LOGGING_CATEGORY(physics_logging_category)
Q_DECLARE_LOGGING_CATEGORY(atoms_logging_category)
Q_DECLARE_LOGGING_CATEGORY(ui_logging_category)


namespace debug
{
    void setup_logging();
}