#include "settings_functions.h"

#include <QSettings>

void ui::load_settings()
{
    
    QSettings developer_settings(
            QSettings::IniFormat,
            QSettings::UserScope,
            "STFC",
            "developer_settings");

    if (!developer_settings.contains("first_run") || developer_settings.value("first_run").toBool())
    {
        developer_settings.setValue("first_run", false);
        developer_settings.setValue("ui_debug_logging_enabled", true);
        developer_settings.setValue("simulation_debug_logging_enabled", true);
        developer_settings.setValue("geometry_debug_logging_enabled", true);
        developer_settings.setValue("physics_debug_logging_enabled", true);
        developer_settings.setValue("atoms_debug_logging_enabled", true);

        developer_settings.sync();
    }
    else
    {
        developer_settings.sync();
    }
}