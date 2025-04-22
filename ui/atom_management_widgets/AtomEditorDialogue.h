\
#ifndef ATOMEDITORDIALOGUE_H
#define ATOMEDITORDIALOGUE_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "../../atoms/Atom.h" // Assuming Atom.h is in the atoms directory relative to the project root

class AtomEditorDialogue : public QDialog {
    Q_OBJECT

public:
    explicit AtomEditorDialogue(Atom* atom, QWidget* parent = nullptr);
    ~AtomEditorDialogue() override = default;


    void applyChanges();

private:
    void setupUI();
    void loadAtomData();

    Atom* m_atom; // Pointer to the atom being edited

    // UI Elements
    QDoubleSpinBox* xSpinBox;
    QDoubleSpinBox* ySpinBox;
    QDoubleSpinBox* zSpinBox;
    // Add more fields as needed (e.g., type, mass, velocity)

    QDialogButtonBox* buttonBox;
};

#endif // ATOMEDITORDIALOGUE_H
