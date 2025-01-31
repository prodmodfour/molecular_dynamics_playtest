#include <QApplication>
#include "InitialiserDialog.h"
#include "InitialConfig.h"

int main(int argc, char *argv[])
{
    // Launch Initialiser dialog box
     QApplication app(argc, argv);

    InitialiserDialog dialog;


    // Get Initial Config from initialiser dialog box
    if (dialog.exec() == QDialog::Accepted)
    {
        InitialConfig userConfig = dialog.getInitialConfig();
    }
}



// Create atom structure

// Create impact atom

// Initialise and add atoms to atom vector

// Launch simulator in its own thread

// Launch visualiser in its own thread


