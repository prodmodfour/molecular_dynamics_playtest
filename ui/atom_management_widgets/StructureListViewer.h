#pragma once

#include <QListWidget>
#include <QListWidgetItem>
#include <string> 


namespace simulation {
    class StructureList;
}

namespace ui {
    class AtomManager;
}

namespace ui {

class StructureListViewer : public QListWidget
{
    Q_OBJECT

public:
    explicit StructureListViewer(QWidget* parent = nullptr);


    void setStructureList(simulation::StructureList* structureList); 
    void refreshList();
    void setParentAtomManager(ui::AtomManager* atomManager);

signals:
    void structureSelected(const std::string& structureName); 

private slots:
    void onSelectionChanged();

private:
    simulation::StructureList* mStructureList = nullptr; 
    ui::AtomManager* parentAtomManager = nullptr;
};


} // namespace ui
