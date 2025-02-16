#pragma once

#include <QDialog>
#include "InitialParameters.h"

namespace ui {

class InitialiserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InitialiserDialog(QWidget *parent = nullptr);

    // Access the values and construct an InitialConfig
    InitialParameters getInitialParameters() const;

private:
    // Forward-declare Qt widgets or include headers. For brevity, you can just do:
    class QDoubleSpinBox;
    class QSpinBox;

    QDoubleSpinBox *impactEnergySpin;
    QDoubleSpinBox *rCutoffSpin;
    QDoubleSpinBox *timestepSpin;
    QSpinBox       *cubesXSpin;
    QSpinBox       *cubesYSpin;
    QSpinBox       *cubesZSpin;
    QDoubleSpinBox *impactAtomXOffsetSpin;
    QDoubleSpinBox *impactAtomYOffsetSpin;
    QDoubleSpinBox *impactAtomZOffsetSpin;
};

} // namespace ui
