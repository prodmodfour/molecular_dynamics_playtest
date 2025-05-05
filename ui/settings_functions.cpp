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

        developer_settings.sync();
    }
    else
    {
        developer_settings.sync();
    }
}