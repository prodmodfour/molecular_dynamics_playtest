#pragma once

#include <QDialog>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include "InitialConfig.h"

class InitialiserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InitialiserDialog(QWidget *parent = nullptr)
        : QDialog(parent)
    {
        setWindowTitle("Initial Configuration Setup");

        // Create input widgets
        impactEnergySpin = new QDoubleSpinBox(this);
        impactEnergySpin->setRange(0.0, 100);
        impactEnergySpin->setValue(10.0); // example default

        rCutoffSpin = new QDoubleSpinBox(this);
        rCutoffSpin->setRange(0.0, 100);
        rCutoffSpin->setValue(4.0); // example default

        timestepSpin = new QDoubleSpinBox(this);
        timestepSpin->setRange(0.0, 1.0);
        timestepSpin->setDecimals(6);
        timestepSpin->setValue(0.001); // example default

        cubesXSpin = new QSpinBox(this);
        cubesXSpin->setRange(1, 100);
        cubesXSpin->setValue(8); // example default

        cubesYSpin = new QSpinBox(this);
        cubesYSpin->setRange(1, 100);
        cubesYSpin->setValue(8); // example default

        cubesZSpin = new QSpinBox(this);
        cubesZSpin->setRange(1, 100);
        cubesZSpin->setValue(4); // example default

        impactAtomXOffsetSpin = new QDoubleSpinBox(this);
        impactAtomXOffsetSpin->setRange(0.0, 100);
        impactAtomXOffsetSpin->setValue(0.0); // example default

        impactAtomYOffsetSpin = new QDoubleSpinBox(this);
        impactAtomYOffsetSpin->setRange(0.0, 100);
        impactAtomYOffsetSpin->setValue(0.0); // example default

        impactAtomZOffsetSpin = new QDoubleSpinBox(this);
        impactAtomZOffsetSpin->setRange(0.0, 100);
        impactAtomZOffsetSpin->setValue(0.0); // example default

        // Layout for form
        QFormLayout *formLayout = new QFormLayout;
        formLayout->addRow("Impact Energy:", impactEnergySpin);
        formLayout->addRow("R-Cutoff:", rCutoffSpin);
        formLayout->addRow("Timestep Size:", timestepSpin);
        formLayout->addRow("Cubes in X:", cubesXSpin);
        formLayout->addRow("Cubes in Y:", cubesYSpin);
        formLayout->addRow("Cubes in Z:", cubesZSpin);
        formLayout->addRow("Impact Atom X Offset:", impactAtomXOffsetSpin);
        formLayout->addRow("Impact Atom Y Offset:", impactAtomYOffsetSpin);
        formLayout->addRow("Impact Atom Z Offset:", impactAtomZOffsetSpin);


        // OK/Cancel buttons
        QDialogButtonBox *buttonBox = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

        connect(buttonBox, &QDialogButtonBox::accepted,
                this, &InitialiserDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected,
                this, &InitialiserDialog::reject);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addLayout(formLayout);
        mainLayout->addWidget(buttonBox);

        setLayout(mainLayout);
    }

    // Access the values and construct an InitialConfig
    InitialConfig getInitialConfig() const
    {
        return InitialConfig(
            impactEnergySpin->value(),
            rCutoffSpin->value(),
            timestepSpin->value(),
            cubesXSpin->value(),
            cubesYSpin->value(),
            cubesZSpin->value(),
            impactAtomXOffsetSpin->value(),
            impactAtomYOffsetSpin->value(),
            impactAtomZOffsetSpin->value()
        );
    }

private:
    QDoubleSpinBox *impactEnergySpin;
    QDoubleSpinBox *rCutoffSpin;
    QDoubleSpinBox *timestepSpin;
    QSpinBox *cubesXSpin;
    QSpinBox *cubesYSpin;
    QSpinBox *cubesZSpin;
    QDoubleSpinBox *impactAtomXOffsetSpin;
    QDoubleSpinBox *impactAtomYOffsetSpin;
    QDoubleSpinBox *impactAtomZOffsetSpin;
};