#pragma once

#include <QMainWindow>
#include "../../atoms/Atom.h"
#include <vector>

class QPushButton;


namespace ui {
    class MDVisualiser;
    class AtomVTKPreview;
}

namespace ui
{

class AtomManager : public QMainWindow
{
    Q_OBJECT

public:
    AtomManager(QWidget* parent = nullptr);
    void onCloseButtonClicked();
    void setParentMDVisualiser(MDVisualiser* visualiser);

    std::vector<Atom> new_atoms;


    


private:
    QPushButton* mCloseButton;
    MDVisualiser* parentMDVisualiser; 

    AtomVTKPreview* mAtomVTKPreview;

};

} // namespace ui