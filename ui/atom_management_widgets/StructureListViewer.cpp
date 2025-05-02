#include "StructureListViewer.h"
#include "../../simulation/StructureList.h"
#include <vector>
#include <string>

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


    std::vector<std::string> names = mStructureList->get_all_structure_names(); // Assuming this method exists and is const

    // Populate the QListWidget
    for (std::string& name : names) {

        new QListWidgetItem(QString::fromStdString(name), this); 
    }
}


void StructureListViewer::onSelectionChanged()
{
    QList<QListWidgetItem*> selectedItems = this->selectedItems();
    if (!selectedItems.isEmpty())
    {

        std::string selectedName = selectedItems.first()->text().toStdString(); 

        parentAtomManager->mAtomVTKPreview->structures_to_display.clear();
        parentAtomManager->mAtomVTKPreview->structures_to_display.push_back(selectedName);
        parentAtomManager->mAtomVTKPreview->updateAtoms();
    }
        


void StructureListViewer::setParentAtomManager(ui::AtomManager* atomManager)
{
    parentAtomManager = atomManager;
}

} // namespace ui
