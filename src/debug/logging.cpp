#include "logging.h"

#include <QSettings>

// I was tempted to implement a setting through QSettings that would toggle logging on or off.
// This is unnecessary.
// If the project is compiled in Release instead of Debug, the logging functionality will be stripped off at compile time.
// If you don't need logging functionality, then you're essentially testing a release build.

Q_LOGGING_CATEGORY(ui_logging_category, "ui")
Q_LOGGING_CATEGORY(simulation_logging_category, "simulation")
Q_LOGGING_CATEGORY(geometry_logging_category, "geometry")
Q_LOGGING_CATEGORY(physics_logging_category, "physics")
Q_LOGGING_CATEGORY(atoms_logging_category, "atoms")

namespace debug
{
    void setup_logging()
    {
        QSettings developer_settings(
                QSettings::IniFormat,
                QSettings::UserScope,
                "STFC",
                "developer_settings");

        if (developer_settings.value("ui_debug_logging_enabled").toBool())
        {
            QLoggingCategory::setFilterRules(QStringLiteral("ui.*=true"));
        }

        if (developer_settings.value("simulation_debug_logging_enabled").toBool())
        {
            QLoggingCategory::setFilterRules(QStringLiteral("simulation.*=true"));
        }

        if (developer_settings.value("geometry_debug_logging_enabled").toBool())
        {
            QLoggingCategory::setFilterRules(QStringLiteral("geometry.*=true"));
        }

        if (developer_settings.value("physics_debug_logging_enabled").toBool())
        {
            QLoggingCategory::setFilterRules(QStringLiteral("physics.*=true"));
        }

        if (developer_settings.value("atoms_debug_logging_enabled").toBool())
        {
            QLoggingCategory::setFilterRules(QStringLiteral("atoms.*=true"));
        }

    }
}

