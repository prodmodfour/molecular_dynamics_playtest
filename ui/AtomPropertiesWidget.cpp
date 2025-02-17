#include "AtomPropertiesWidget.h"

ui::AtomPropertiesWidget::AtomPropertiesWidget(QWidget* parent)
    : QWidget(parent)
{
    mLayout = new QVBoxLayout(this);

    mLabel = new QLabel("Atom Properties", this);
    mLayout->addWidget(mLabel);

    mXSpinBox = new QDoubleSpinBox(this);
    mLayout->addWidget(mXSpinBox);

    mYSpinBox = new QDoubleSpinBox(this);
    mLayout->addWidget(mYSpinBox);

    mZSpinBox = new QDoubleSpinBox(this);
    mLayout->addWidget(mZSpinBox);

    mFxSpinBox = new QDoubleSpinBox(this);
    mLayout->addWidget(mFxSpinBox);

    mFySpinBox = new QDoubleSpinBox(this);
    mLayout->addWidget(mFySpinBox);

    mFzSpinBox = new QDoubleSpinBox(this);
    mLayout->addWidget(mFzSpinBox);

    mVxSpinBox = new QDoubleSpinBox(this);
    mLayout->addWidget(mVxSpinBox);

    mVySpinBox = new QDoubleSpinBox(this);
    mLayout->addWidget(mVySpinBox);

    mVzSpinBox = new QDoubleSpinBox(this);
    mLayout->addWidget(mVzSpinBox);

    mRadiusSpinBox = new QDoubleSpinBox(this);
    mLayout->addWidget(mRadiusSpinBox);

    mMassSpinBox = new QDoubleSpinBox(this);
    mLayout->addWidget(mMassSpinBox);

    mKineticEnergySpinBox = new QDoubleSpinBox(this);
    mLayout->addWidget(mKineticEnergySpinBox);

    mTypeLineEdit = new QLineEdit(this);
    mLayout->addWidget(mTypeLineEdit);

    mNameLineEdit = new QLineEdit(this);
    mLayout->addWidget(mNameLineEdit);

    mModifyButton = new QPushButton("Modify", this);
    mLayout->addWidget(mModifyButton);

    update_properties_displayed();

    connect(mModifyButton, &QPushButton::clicked, this, &AtomPropertiesWidget::modifyAtom);
    
}

void ui::AtomPropertiesWidget::set_atom(atoms::Atom* atom)
{
    mAtom = atom;
    update_properties_displayed();
}

void ui::AtomPropertiesWidget::update_properties_displayed()
{
    mXSpinBox->setValue(mAtom->x);
    mYSpinBox->setValue(mAtom->y);
    mZSpinBox->setValue(mAtom->z);
    mFxSpinBox->setValue(mAtom->fx);
    mFySpinBox->setValue(mAtom->fy);
    mFzSpinBox->setValue(mAtom->fz);
    mVxSpinBox->setValue(mAtom->vx);
    mVySpinBox->setValue(mAtom->vy);
    mVzSpinBox->setValue(mAtom->vz);
    mRadiusSpinBox->setValue(mAtom->radius);   
    mMassSpinBox->setValue(mAtom->mass);
    mKineticEnergySpinBox->setValue(mAtom->kinetic_energy);
    mTypeLineEdit->setText(mAtom->type.c_str());
    mNameLineEdit->setText(mAtom->name.c_str());
}

void ui::AtomPropertiesWidget::modify_atom()
{
    mAtom->x = mXSpinBox->value();
    mAtom->y = mYSpinBox->value();
    mAtom->z = mZSpinBox->value();
    mAtom->fx = mFxSpinBox->value();
    mAtom->fy = mFySpinBox->value();
    mAtom->fz = mFzSpinBox->value();
    mAtom->vx = mVxSpinBox->value();
    mAtom->vy = mVySpinBox->value();
    mAtom->vz = mVzSpinBox->value();
    mAtom->radius = mRadiusSpinBox->value();
    mAtom->mass = mMassSpinBox->value();
    mAtom->kinetic_energy = mKineticEnergySpinBox->value();
    mAtom->type = mTypeLineEdit->text().toStdString();
    mAtom->name = mNameLineEdit->text().toStdString();
    
}

