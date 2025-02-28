#pragma once

#include <QDialog>
#include "../simulation/InitialParameters.h"
#include <QDoubleSpinBox>
#include <QSpinBox>

namespace ui {

class InitialiserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InitialiserDialog(QWidget *parent = nullptr);

    // Access the values and construct an InitialConfig
    simulation::InitialParameters getInitialParameters() const;

private:


    QDoubleSpinBox *impactEnergySpin;
    QDoubleSpinBox *rCutoffSpin;
    QDoubleSpinBox *timestepSpin;
    QSpinBox       *cubesXSpin;
    QSpinBox       *cubesYSpin;
    QSpinBox       *cubesZSpin;
    QDoubleSpinBox *impactAtomXOffsetSpin;
    QDoubleSpinBox *impactAtomYOffsetSpin;
    QDoubleSpinBox *impactAtomZOffsetSpin;
    QSpinBox       *numberOfTimestepsSpin;
    QDoubleSpinBox *atomSpacingSpin;
};

} // namespace ui
