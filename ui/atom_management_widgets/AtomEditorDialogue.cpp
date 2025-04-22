#include "AtomEditorDialogue.h"

#include <iostream>

AtomEditorDialogue::AtomEditorDialogue(Atom* atom, QWidget* parent)
    : QDialog(parent), m_atom(atom) {
    setWindowTitle("Edit Atom");
    setupUI();
    loadAtomData();
}

void AtomEditorDialogue::setupUI() {
    auto mainLayout = new QVBoxLayout(this);
    auto formLayout = new QFormLayout();

    // Position Spin Boxes
    xSpinBox = new QDoubleSpinBox(this);
    ySpinBox = new QDoubleSpinBox(this);
    zSpinBox = new QDoubleSpinBox(this);

    // Set appropriate ranges and step sizes if needed
    xSpinBox->setRange(-10000.0, 10000.0);
    ySpinBox->setRange(-10000.0, 10000.0);
    zSpinBox->setRange(-10000.0, 10000.0);
    xSpinBox->setDecimals(4); // Example precision
    ySpinBox->setDecimals(4);
    zSpinBox->setDecimals(4);

    formLayout->addRow("Position X (Å):", xSpinBox);
    formLayout->addRow("Position Y (Å):", ySpinBox);
    formLayout->addRow("Position Z (Å):", zSpinBox);

    // Add more UI elements for other Atom properties here (e.g., type, velocity)
    // Example for Atom Type (assuming you have a way to represent types)
    // atomTypeComboBox = new QComboBox(this);
    // atomTypeComboBox->addItem("Copper");
    // atomTypeComboBox->addItem("Argon");
    // formLayout->addRow("Atom Type:", atomTypeComboBox);

    mainLayout->addLayout(formLayout);

    // Dialog Buttons
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);

    // Connect signals for immediate update (optional)
    // connect(xSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &AtomEditorDialogue::onPositionChanged);
    // connect(ySpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &AtomEditorDialogue::onPositionChanged);
    // connect(zSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &AtomEditorDialogue::onPositionChanged);

    // Accept action can trigger the update
    connect(this, &QDialog::accepted, this, [this]() {
        if (m_atom) {
            m_atom->position.x = xSpinBox->value();
            m_atom->position.y = ySpinBox->value();
            m_atom->position.z = zSpinBox->value();
            // Update other properties here
            std::cout << "Atom position updated!" << std::endl; // Debug message
        }
    });
}

void AtomEditorDialogue::loadAtomData() {
    if (m_atom) {
        xSpinBox->setValue(m_atom->position.x);
        ySpinBox->setValue(m_atom->position.y);
        zSpinBox->setValue(m_atom->position.z);

        // Load other Atom properties into UI elements
        // Example:
        // if (m_atom->type == AtomType::Copper) {
        //     atomTypeComboBox->setCurrentIndex(0);
        // } else if (m_atom->type == AtomType::Argon) {
        //     atomTypeComboBox->setCurrentIndex(1);
        // }
    } else {
        // Handle the case where the atom pointer is null (e.g., disable fields)
        std::cerr << "Error: Atom pointer is null in AtomEditorDialogue." << std::endl;
        xSpinBox->setEnabled(false);
        ySpinBox->setEnabled(false);
        zSpinBox->setEnabled(false);
        // Disable other fields
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}

