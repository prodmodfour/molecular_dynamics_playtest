#pragma once

#include "AtomStructureInserter.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QCheckBox>

#include <iostream>
#include <stdexcept>

AtomStructureInserter::AtomStructureInserter(QWidget *parent) : QDialog(parent) {
  setWindowTitle("Atom Structure Inserter");

  // Main layout
  auto mainLayout = new QVBoxLayout(this);

  // Structure type selection
  auto structureTypeGroup = new QGroupBox("Structure Type", this);
  auto structureTypeLayout = new QVBoxLayout(structureTypeGroup);
  structureTypeComboBox = new QComboBox(this);
  structureTypeComboBox->addItem("Single Atom");
  structureTypeComboBox->addItem("FCC Crystal");
  structureTypeLayout->addWidget(structureTypeComboBox);
  mainLayout->addWidget(structureTypeGroup);

  // Atom properties
  auto atomPropertiesGroup = new QGroupBox("Atom Properties", this);
  auto atomPropertiesLayout = new QFormLayout(atomPropertiesGroup);
  atomTypeComboBox = new QComboBox(this);
  atomTypeComboBox->addItem("Copper");
  atomTypeComboBox->addItem("Argon");
  atomPropertiesLayout->addRow("Atom Type:", atomTypeComboBox);

  atomRadiusSpinBox = new QDoubleSpinBox(this);
  atomRadiusSpinBox->setRange(0.1, 10.0); // Example range
  atomRadiusSpinBox->setValue(1.28);     // Default value
  atomPropertiesLayout->addRow("Atom Radius (Å):", atomRadiusSpinBox);

  centerXSpinBox = new QDoubleSpinBox(this);
  centerYSpinBox = new QDoubleSpinBox(this);
  centerZSpinBox = new QDoubleSpinBox(this);
  centerXSpinBox->setValue(0.0);
  centerYSpinBox->setValue(0.0);
  centerZSpinBox->setValue(0.0);
  atomPropertiesLayout->addRow("Center X (Å):", centerXSpinBox);
  atomPropertiesLayout->addRow("Center Y (Å):", centerYSpinBox);
  atomPropertiesLayout->addRow("Center Z (Å):", centerZSpinBox);
  mainLayout->addWidget(atomPropertiesGroup);

  // FCC Crystal properties (initially hidden)
  fccPropertiesGroup = new QGroupBox("FCC Crystal Properties", this);
  auto fccPropertiesLayout = new QFormLayout(fccPropertiesGroup);
  cubesXSpinBox = new QSpinBox(this);
  cubesYSpinBox = new QSpinBox(this);
  cubesZSpinBox = new QSpinBox(this);
  cubesXSpinBox->setRange(1, 100); 
  cubesYSpinBox->setRange(1, 100);
  cubesZSpinBox->setRange(1, 100);

  // Default values
  cubesXSpinBox->setValue(1); 
  cubesYSpinBox->setValue(1);
  cubesZSpinBox->setValue(1);
  fccPropertiesLayout->addRow("Cubes in X:", cubesXSpinBox);
  fccPropertiesLayout->addRow("Cubes in Y:", cubesYSpinBox);
  fccPropertiesLayout->addRow("Cubes in Z:", cubesZSpinBox);

  atomSpacingSpinBox = new QDoubleSpinBox(this);
  atomSpacingSpinBox->setRange(0.1, 10.0); // Example range
  atomSpacingSpinBox->setValue(3.61);     // Default value
  fccPropertiesLayout->addRow("Atom Spacing (Å):", atomSpacingSpinBox);
  mainLayout->addWidget(fccPropertiesGroup);
  fccPropertiesGroup->hide();

  // Kinetic Energy
  kineticEnergyGroup = new QGroupBox("Kinetic Energy", this);
  auto kineticEnergyLayout = new QFormLayout(kineticEnergyGroup);
  applyKineticEnergyCheckBox = new QCheckBox("Apply kinetic energy to structure", this);
  kineticEnergyLayout->addWidget(applyKineticEnergyCheckBox);

  kineticEnergySpinBox = new QDoubleSpinBox(this);
  kineticEnergySpinBox->setRange(0.0, 1000.0); // Example range
  kineticEnergySpinBox->setValue(10.0);      // Default value
  kineticEnergyLayout->addRow("Kinetic Energy (eV):", kineticEnergySpinBox);

  targetXSpinBox = new QDoubleSpinBox(this);
  targetYSpinBox = new QDoubleSpinBox(this);
  targetZSpinBox = new QDoubleSpinBox(this);
  targetXSpinBox->setValue(0.0);
  targetYSpinBox->setValue(0.0);
  targetZSpinBox->setValue(-1.0);
  kineticEnergyLayout->addRow("Target X (Å):", targetXSpinBox);
  kineticEnergyLayout->addRow("Target Y (Å):", targetYSpinBox);
  kineticEnergyLayout->addRow("Target Z (Å):", targetZSpinBox);

  offsetXSpinBox = new QDoubleSpinBox(this);
  offsetYSpinBox = new QDoubleSpinBox(this);
  offsetZSpinBox = new QDoubleSpinBox(this);
  offsetXSpinBox->setValue(0.0);
  offsetYSpinBox->setValue(0.0);
  offsetZSpinBox->setValue(1.0);
  kineticEnergyLayout->addRow("Offset X (Å):", offsetXSpinBox);
  kineticEnergyLayout->addRow("Offset Y (Å):", offsetYSpinBox);
  kineticEnergyLayout->addRow("Offset Z (Å):", offsetZSpinBox);

  mainLayout->addWidget(kineticEnergyGroup);
  kineticEnergyGroup->hide();

  // Connect signals and slots
  connect(structureTypeComboBox, &QComboBox::currentIndexChanged, this, &AtomStructureInserter::onStructureTypeChanged);
  connect(applyKineticEnergyCheckBox, &QCheckBox::toggled, this, &AtomStructureInserter::onApplyKineticEnergyChanged);

  // Initially update visibility
  onStructureTypeChanged(structureTypeComboBox->currentIndex());
  onApplyKineticEnergyChanged(applyKineticEnergyCheckBox->isChecked());

  // Dialog buttons
  auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
  connect(buttonBox, &QDialogButtonBox::accepted, this, &AtomStructureInserter::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &AtomStructureInserter::reject);
  mainLayout->addWidget(buttonBox);
}

void AtomStructureInserter::onStructureTypeChanged(int index) {
  fccPropertiesGroup->setVisible(structureTypeComboBox->itemText(index) == "FCC Crystal");
}

void AtomStructureInserter::onApplyKineticEnergyChanged(bool checked) {
  kineticEnergyGroup->setVisible(checked);
}

AtomStructureParameters AtomStructureInserter::getParameters() const {
  AtomStructureParameters params;
  params.structureType = structureTypeComboBox->currentText() == "Single Atom" ? StructureType::SingleAtom : StructureType::FCCCrystal;
  params.atomType = atomTypeComboBox->currentText() == "Copper" ? AtomType::Copper : AtomType::Argon;
  params.atomRadius = atomRadiusSpinBox->value();
  params.center = {centerXSpinBox->value(), centerYSpinBox->value(), centerZSpinBox->value()};

  if (params.structureType == StructureType::FCCCrystal) {
    params.fccParams.cubes = {cubesXSpinBox->value(), cubesYSpinBox->value(), cubesZSpinBox->value()};
    params.fccParams.spacing = atomSpacingSpinBox->value();
  }

  params.applyKineticEnergy = applyKineticEnergyCheckBox->isChecked();
  if (params.applyKineticEnergy) {
    params.kineticEnergyParams.kineticEnergy = kineticEnergySpinBox->value();
    params.kineticEnergyParams.targetCoordinates = {targetXSpinBox->value(), targetYSpinBox->value(), targetZSpinBox->value()};
    params.kineticEnergyParams.offset = {offsetXSpinBox->value(), offsetYSpinBox->value(), offsetZSpinBox->value()};
  }

  return params;
}
