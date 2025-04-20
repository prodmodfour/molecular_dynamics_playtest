#pragma once

#include <QDialog>
#include <QString> // Include QString

// Forward declarations for Qt classes
class QComboBox;
class QDoubleSpinBox;
class QSpinBox;
class QCheckBox;
class QLabel;
class QGridLayout;
class QPushButton;
class QGroupBox;

// Define necessary enums and structs based on AtomStructureInserter.cpp usage
enum class StructureType { SingleAtom, FCCCrystal };
enum class AtomType { Copper, Argon }; // Assuming these are the types

// Define a simple struct for 3D coordinates/vectors
struct Vector3D {
    double x = 0.0, y = 0.0, z = 0.0;
};

struct FCCParams {
    int cubesX = 1, cubesY = 1, cubesZ = 1; // Match spinbox type (int)
    double spacing = 0.0;
};

struct KineticEnergyParams {
    double kineticEnergy = 0.0;
    Vector3D targetCoordinates;
    Vector3D offset;
};

struct AtomStructureParameters {
    StructureType structureType = StructureType::SingleAtom;
    AtomType atomType = AtomType::Copper;
    double atomRadius = 0.0;
    Vector3D center; // Represents the center coordinates
    FCCParams fccParams; // Nested struct for FCC params
    bool applyKineticEnergy = false;
    KineticEnergyParams kineticEnergyParams; // Nested struct for KE params
};


class AtomStructureInserter : public QDialog {
    Q_OBJECT

public:
    AtomStructureInserter(QWidget *parent = nullptr);
    // Remove destructor if not custom implemented: ~AtomStructureInserter() override;

    // Moved getParameters inside the class
    AtomStructureParameters getParameters() const;

private slots:
    // Corrected signatures
    void onStructureTypeChanged(int index);
    void onApplyKineticEnergyChanged(bool checked);

private:
    QComboBox *structureTypeComboBox;
    QComboBox *atomTypeComboBox;
    QDoubleSpinBox *atomRadiusSpinBox;
    QDoubleSpinBox *centerXSpinBox, *centerYSpinBox, *centerZSpinBox;

    // FCC Crystal Specific - Renamed member
    QGroupBox *fccPropertiesGroup; // <--- Renamed
    QSpinBox *cubesXSpinBox, *cubesYSpinBox, *cubesZSpinBox;
    QDoubleSpinBox *atomSpacingSpinBox;

    QCheckBox *applyKineticEnergyCheckBox;
    QGroupBox *kineticEnergyGroup;
    QDoubleSpinBox *kineticEnergySpinBox;
    QDoubleSpinBox *targetXSpinBox, *targetYSpinBox, *targetZSpinBox;
    QDoubleSpinBox *offsetXSpinBox, *offsetYSpinBox, *offsetZSpinBox;

    // Added missing members from cpp constructor
    QPushButton *okButton, *cancelButton;

    // Removed unused private helper declarations if they aren't defined in cpp
    // void createLayout();
    // void updateKineticEnergyGroupVisibility(bool visible);
    // void updateFCCGroupVisibility(bool visible);
    // void calculateOffset();
};
