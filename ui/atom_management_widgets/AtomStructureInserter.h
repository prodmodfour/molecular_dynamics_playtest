#pragma once

#include <QDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>

struct AtomStructureParametersParameters {
    QString structureType;
    QString atomType;
    double atomRadius;
    double centerX, centerY, centerZ;
    int cubesX, cubesY, cubesZ;
    double atomSpacing;
    bool applyKineticEnergy;
    double kineticEnergy;
    double targetX, targetY, targetZ;
    double offsetX, offsetY, offsetZ;
};

AtomStructureParameters getParameters() const;

class AtomStructureInserter : public QDialog {
    Q_OBJECT

public:
    AtomStructureInserter(QWidget *parent = nullptr);
    ~AtomStructureInserter() override;



private slots:
    void onStructureTypeChanged(const QString &text);
    void onApplyKineticEnergyChanged(int state);

private:
    QComboBox *structureTypeComboBox;
    QComboBox *atomTypeComboBox;
    QDoubleSpinBox *atomRadiusSpinBox;
    QDoubleSpinBox *centerXSpinBox, *centerYSpinBox, *centerZSpinBox;

    // FCC Crystal Specific
    QGroupBox *fccCrystalGroup;
    QSpinBox *cubesXSpinBox, *cubesYSpinBox, *cubesZSpinBox;
    QDoubleSpinBox *atomSpacingSpinBox;

    QCheckBox *applyKineticEnergyCheckBox;
    QGroupBox *kineticEnergyGroup;
    QDoubleSpinBox *kineticEnergySpinBox;
    QDoubleSpinBox *targetXSpinBox, *targetYSpinBox, *targetZSpinBox;
    QDoubleSpinBox *offsetXSpinBox, *offsetYSpinBox, *offsetZSpinBox;

    QPushButton *okButton, *cancelButton;

    void createLayout();
    void updateKineticEnergyGroupVisibility(bool visible);
    void updateFCCGroupVisibility(bool visible);
    void calculateOffset();
};


