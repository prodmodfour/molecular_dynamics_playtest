#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QMainWindow>
#include "SimulationConfig.h"
#include "Atom.h"
#include "AtomGenerator.h"
#include "SimulationData.h"
#include "MDVisualiser.h"


class QDoubleSpinBox;
class QSpinBox;
class QCheckBox;
class QPushButton;

class Launcher : public QMainWindow {
    Q_OBJECT

public:
    explicit Launcher(QWidget* parent = nullptr);

private slots:
    void startSimulation();

private:
    // UI elements
    QDoubleSpinBox* atomMassSpinBox;
    QSpinBox* cubesInXSpinBox;
    QSpinBox* cubesInYSpinBox;
    QSpinBox* cubesInZSpinBox;
    QDoubleSpinBox* atomSpacingSpinBox;
    QDoubleSpinBox* energyAppliedSpinBox;
    QCheckBox* addImpactCheckBox;
    QPushButton* startButton;
};

#endif  // MAINWINDOW_H