#ifndef SIMULATIONLAUNCHER_H
#define SIMULATIONLAUNCHER_H

#include <QDialog>

class SimulationLauncher : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit SimulationLauncher(QWidget *parent = nullptr)
        : QDialog(parent)
    {
        // Any common initialization code can go here
    }
    
    // Virtual destructor
    virtual ~SimulationLauncher() = default;

    // Pure virtual method that derived classes must implement
    virtual void Launch() = 0;
};

#endif // SIMULATIONLAUNCHER_H
