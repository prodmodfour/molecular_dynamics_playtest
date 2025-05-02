#include "NewStructureDialog.h"

#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <iostream>
#include <QLineEdit>

namespace ui
{

NewStructureDialog::NewStructureDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("New Structure"));
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setText(tr("New Structure"));

    m_structureCB  = new QComboBox(this);
    m_structureCB->addItems({QStringLiteral("fcc"),
                             QStringLiteral("bcc")});

    auto makeIntSpin = [this]() {
        auto *sb = new QSpinBox(this);
        sb->setRange(1, 1000);
        sb->setValue(4);
        return sb;
    };
    m_cubesXSB = makeIntSpin();
    m_cubesYSB = makeIntSpin();
    m_cubesZSB = makeIntSpin();

    m_atomTypeCB = new QComboBox(this);
    m_atomTypeCB->addItem(QStringLiteral("Cu"));

    m_spacingDSB = new QDoubleSpinBox(this);
    m_spacingDSB->setDecimals(3);
    m_spacingDSB->setRange(3.61, 1000.0);
    m_spacingDSB->setValue(3.61);
    m_spacingDSB->setSuffix(QStringLiteral(" Å"));

    m_offsetXDSB = new QDoubleSpinBox(this);
    m_offsetXDSB->setDecimals(3);
    m_offsetXDSB->setRange(-1000.0, 1000.0);
    m_offsetXDSB->setValue(0.0);
    m_offsetXDSB->setSuffix(QStringLiteral(" Å"));

    m_offsetYDSB = new QDoubleSpinBox(this);
    m_offsetYDSB->setDecimals(3);
    m_offsetYDSB->setRange(-1000.0, 1000.0);
    m_offsetYDSB->setValue(0.0);
    m_offsetYDSB->setSuffix(QStringLiteral(" Å"));

    m_offsetZDSB = new QDoubleSpinBox(this);
    m_offsetZDSB->setDecimals(3);
    m_offsetZDSB->setRange(-1000.0, 1000.0);
    m_offsetZDSB->setValue(0.0);
    m_offsetZDSB->setSuffix(QStringLiteral(" Å"));
    

    auto *form = new QFormLayout;
    form->addRow(tr("Structure name:"),  m_nameEdit);
    form->addRow(tr("Structure type:"),  m_structureCB);
    form->addRow(tr("Cubes in X:"),      m_cubesXSB);
    form->addRow(tr("Cubes in Y:"),      m_cubesYSB);
    form->addRow(tr("Cubes in Z:"),      m_cubesZSB);
    form->addRow(tr("Atom type:"),       m_atomTypeCB);
    form->addRow(tr("Atom spacing:"),    m_spacingDSB);
    form->addRow(tr("Offset X:"),        m_offsetXDSB);
    form->addRow(tr("Offset Y:"),        m_offsetYDSB);
    form->addRow(tr("Offset Z:"),        m_offsetZDSB);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok
                                         | QDialogButtonBox::Cancel,
                                         Qt::Horizontal, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &NewStructureDialog::onAccept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto *vbox = new QVBoxLayout(this);
    vbox->addLayout(form);
    vbox->addWidget(buttons);
    setLayout(vbox);
}

void NewStructureDialog::onAccept()
{
    m_params.structureName = m_nameEdit->text().trimmed();
    m_params.structureType = m_structureCB->currentText();
    m_params.cubesX        = m_cubesXSB->value();
    m_params.cubesY        = m_cubesYSB->value();
    m_params.cubesZ        = m_cubesZSB->value();
    m_params.atomType      = m_atomTypeCB->currentText();
    m_params.atomSpacing   = m_spacingDSB->value();
    m_params.offsetX       = m_offsetXDSB->value();
    m_params.offsetY       = m_offsetYDSB->value();
    m_params.offsetZ       = m_offsetZDSB->value();

    accept();
}


QString NewStructureDialog::structureType() const { return m_structureCB->currentText(); }
int     NewStructureDialog::cubesX()        const { return m_cubesXSB->value(); }
int     NewStructureDialog::cubesY()        const { return m_cubesYSB->value(); }
int     NewStructureDialog::cubesZ()        const { return m_cubesZSB->value(); }
QString NewStructureDialog::atomType()      const { return m_atomTypeCB->currentText(); }
double  NewStructureDialog::atomSpacing()   const { return m_spacingDSB->value(); }

} // namespace ui
