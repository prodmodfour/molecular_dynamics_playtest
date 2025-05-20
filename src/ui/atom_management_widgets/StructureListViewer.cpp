#include "StructureListViewer.h"
#include "../../simulation/StructureList.h"
#include <vector>
#include <string>
#include "AtomManager.h"
#include "AtomVTKPreview.h"
#include <iostream>

namespace ui {


StructureListViewer::StructureListViewer(QWidget* parent)
    : QListWidget(parent)
{

    connect(this, &QListWidget::itemSelectionChanged, this, &StructureListViewer::onSelectionChanged);
}

void StructureListViewer::setStructureList(simulation::StructureList* structureList)
{
    mStructureList = structureList;
    refreshList(); 
}

void StructureListViewer::refreshList()
{
 
    clear(); 

    if (!mStructureList) {
        return;
    }


    std::vector<std::string> names = mStructureList->get_all_structure_names(); 

    for (std::string& name : names) {

        new QListWidgetItem(QString::fromStdString(name), this); 
    }
}


void StructureListViewer::onSelectionChanged()
{
    QList<QListWidgetItem*> selectedItems = this->selectedItems();
    if (!selectedItems.isEmpty())
    {
        std::cout << "Structure Selection changed" << std::endl;
        std::string selectedName = selectedItems.first()->text().toStdString(); 
        std::cout << "Selected Structure: " << selectedName << std::endl;

        parentAtomManager->get_atom_vtk_preview()->get_structures_to_display().clear();
        parentAtomManager->get_atom_vtk_preview()->get_structures_to_display().push_back(selectedName);
        parentAtomManager->get_atom_vtk_preview()->updateAtoms();
    }
    else
    {
        std::cout << "No Structure Selected" << std::endl;
        parentAtomManager->get_atom_vtk_preview()->get_structures_to_display().clear();
        parentAtomManager->get_atom_vtk_preview()->updateAtoms();
    }
}
        


void StructureListViewer::setParentAtomManager(ui::AtomManager* atomManager)
{
    parentAtomManager = atomManager;
}



} // namespace ui
