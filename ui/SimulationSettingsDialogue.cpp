#include "SimulationSettingsDialogue.h"
#include "SharedData.h"

#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <limits>
#include <mutex>
#include <iostream>


namespace ui
{

SimulationSettingsDialogue::SimulationSettingsDialogue(QWidget* parent, SharedData* shared_data)
    : QDialog(parent), shared_data(shared_data)
{
    setWindowTitle("Simulation Settings");
    setSharedData(shared_data);

    // --- Spin boxes ---------------------------------------------------------
    auto* rCutoffSpin = new QDoubleSpinBox(this);
    rCutoffSpin->setObjectName("rCutoffSpin");
    rCutoffSpin->setDecimals(6);
    rCutoffSpin->setSuffix(" Å");               // Angstroms
    rCutoffSpin->setMinimum(0.0);
    rCutoffSpin->setMaximum(std::numeric_limits<double>::max());
    rCutoffSpin->setValue(shared_data->config.r_cutoff);

    auto* timestepSpin = new QDoubleSpinBox(this);
    timestepSpin->setObjectName("timestepSpin");
    timestepSpin->setDecimals(6);
    timestepSpin->setSuffix(" ps");             // Picoseconds
    timestepSpin->setMinimum(0.0);
    timestepSpin->setMaximum(std::numeric_limits<double>::max());
    timestepSpin->setValue(shared_data->config.timestep_size);

    // --- Layout -------------------------------------------------------------
    auto* formLayout = new QFormLayout;
    formLayout->addRow(tr("Cut‑off radius (Å):"), rCutoffSpin);
    formLayout->addRow(tr("Timestep size (ps):"), timestepSpin);

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Apply | QDialogButtonBox::Cancel,
                                           Qt::Horizontal, this);

    connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked,
            this, &SimulationSettingsDialogue::onApplyClicked);
    connect(buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked,
            this, &SimulationSettingsDialogue::onCancelClicked);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}

void SimulationSettingsDialogue::setSharedData(SharedData* shared_data)
{
    this->shared_data = shared_data;
}

void SimulationSettingsDialogue::applyChanges()
{
    auto* rCutoffSpin  = findChild<QDoubleSpinBox*>("rCutoffSpin");
    auto* timestepSpin = findChild<QDoubleSpinBox*>("timestepSpin");

    std::lock_guard<std::mutex> lock(shared_data->mutex);
    shared_data->indexes_of_timesteps_edited_by_ui.push_back(0);
    shared_data->config.r_cutoff = rCutoffSpin->value();
    shared_data->config.timestep_size = timestepSpin->value();
    

}

void SimulationSettingsDialogue::onApplyClicked()
{
    auto* rCutoffSpin  = findChild<QDoubleSpinBox*>("rCutoffSpin");
    auto* timestepSpin = findChild<QDoubleSpinBox*>("timestepSpin");

    if (!rCutoffSpin || !timestepSpin)
    {
        reject();
        return;
    }

    applyChanges();
    accept();
}

void SimulationSettingsDialogue::onCancelClicked()
{
    reject();
}


} // namespace ui
