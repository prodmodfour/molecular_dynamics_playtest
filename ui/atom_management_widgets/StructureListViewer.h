#pragma once

#include <QListWidget>
#include <QListWidgetItem>
#include <string> // Added for std::string

// Forward declaration
namespace simulation {
    class StructureList;
}


namespace ui {

class StructureListViewer : public QListWidget
{
    Q_OBJECT

public:
    explicit StructureListViewer(QWidget* parent = nullptr);

    // Takes a const pointer as it shouldn't modify the list directly, only observe
    void setStructureList(const simulation::StructureList* structureList); 
    void refreshList(); // Added a method to manually refresh the list view

signals:
    // Emits the name of the selected structure
    void structureSelected(const std::string& structureName); 

private slots:
    void onSelectionChanged();

private:
    const simulation::StructureList* mStructureList = nullptr; // Store a const pointer
};


} // namespace ui
