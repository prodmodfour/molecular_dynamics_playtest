#include "StructureListViewer.h"
#include "../../simulation/StructureList.h"
#include <vector>
#include <string>

namespace ui {


StructureListViewer::StructureListViewer(QWidget* parent)
    : QListWidget(parent)
{
    // Connect the itemSelectionChanged signal to our custom slot
    connect(this, &QListWidget::itemSelectionChanged, this, &StructureListViewer::onSelectionChanged);
}

void StructureListViewer::setStructureList(const simulation::StructureList* structureList)
{
    mStructureList = structureList;
    refreshList(); // Populate the list initially
}

void StructureListViewer::refreshList()
{
    // Clear the existing items
    clear(); 

    if (!mStructureList) {
        return; // No data source set
    }

    // Get the names from the StructureList
    std::vector<std::string> names = mStructureList->get_all_structure_names(); // Assuming this method exists and is const

    // Populate the QListWidget
    for (const std::string& name : names) {
        // Add each structure name as a QListWidgetItem
        new QListWidgetItem(QString::fromStdString(name), this); 
    }
}


void StructureListViewer::onSelectionChanged()
{
    QList<QListWidgetItem*> selectedItems = this->selectedItems();
    if (!selectedItems.isEmpty()) {
        // Get the text (structure name) of the first selected item
        std::string selectedName = selectedItems.first()->text().toStdString(); 
        // Emit the signal with the selected structure's name
        emit structureSelected(selectedName); 
    } else {
         // Optionally emit an empty string or a specific signal if nothing is selected
         // emit structureSelected(""); 
    }
}

} // namespace ui
