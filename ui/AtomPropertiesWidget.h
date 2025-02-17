#pragma once
#include <QObject>
#include <QWidget>
#include "../atoms/Atom.h"

namespace ui {

class AtomPropertiesWidget : public QWidget
{
    Q_OBJECT
public:
    AtomPropertiesWidget(QWidget* parent = nullptr);
    atoms::Atom* mAtom;
    void set_atom(atoms::Atom* atom);
    void update_properties_displayed();
    void modify_atom();

private:
    class QDoubleSpinBox;
    class QSpinBox;
    class QLineEdit;
    class QPushButton;

    QVBoxLayout* mLayout;

    // Properties
    QDoubleSpinBox* mXSpinBox;
    QDoubleSpinBox* mYSpinBox;
    QDoubleSpinBox* mZSpinBox;
    QDoubleSpinBox* mFxSpinBox;
    QDoubleSpinBox* mFySpinBox;
    QDoubleSpinBox* mFzSpinBox;
    QDoubleSpinBox* mVxSpinBox;
    QDoubleSpinBox* mVySpinBox;
    QDoubleSpinBox* mVzSpinBox;
    QDoubleSpinBox* mRadiusSpinBox;
    QDoubleSpinBox* mMassSpinBox;
    QDoubleSpinBox* mKineticEnergySpinBox;
    QLineEdit* mTypeLineEdit;
    QLineEdit* mNameLineEdit;

    QPushButton* mModifyButton;
};

}