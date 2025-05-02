#pragma once

#include <QMainWindow>
#include "../../atoms/Atom.h"
#include <vector>
#include <string>

class QPushButton;


namespace ui {
    class MDVisualiser;
    class AtomVTKPreview;
    class StructureListViewer;
}

namespace simulation {
    class StructureList;
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
    void setStructureList(simulation::StructureList* structureList);

    std::vector<atoms::Atom> new_atoms;
    ui::StructureListViewer* mStructureListViewer;

    AtomVTKPreview* mAtomVTKPreview;

    


private:
    QPushButton* mCloseButton;
    MDVisualiser* parentMDVisualiser; 




};

} // namespace ui