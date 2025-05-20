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

    void onAddStructureButtonClicked();

    std::vector<atoms::Atom> get_new_atoms();
    void set_new_atoms(std::vector<atoms::Atom> new_atoms);
    ui::StructureListViewer* get_structure_list_viewer();
    void set_structure_list_viewer(ui::StructureListViewer* structure_list_viewer);
    AtomVTKPreview* get_atom_vtk_preview();
    MDVisualiser* get_parent_md_visualiser();

private:
    QPushButton* mCloseButton;
    QPushButton* mAddStructureButton;
    MDVisualiser* parentMDVisualiser; 
    
    std::vector<atoms::Atom> new_atoms;
    ui::StructureListViewer* mStructureListViewer;

    AtomVTKPreview* mAtomVTKPreview;






};

} // namespace ui