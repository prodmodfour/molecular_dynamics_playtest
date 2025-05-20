#include "AtomManager.h"
#include "../main_visualiser_widgets/MDVisualiser.h" 
#include "AtomVTKPreview.h"
#include "StructureListViewer.h"
#include "../../simulation/StructureList.h"
#include <QVBoxLayout>
#include <QPushButton>
#include "../../atoms/Atom.h"
#include <vector>
#include "NewStructureDialog.h"
#include "../../atoms/Structure.h"

ui::AtomManager::AtomManager(QWidget* parent) : QMainWindow(parent)
{
    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);


    mAtomVTKPreview = new AtomVTKPreview(central);
    mAtomVTKPreview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(mAtomVTKPreview);

    mStructureListViewer = new StructureListViewer(central);
    mStructureListViewer->setParentAtomManager(this);
    mainLayout->addWidget(mStructureListViewer);

    mAddStructureButton = new QPushButton("Add Structure", central);
    mainLayout->addWidget(mAddStructureButton);
    connect(mAddStructureButton, &QPushButton::clicked, this, &ui::AtomManager::onAddStructureButtonClicked);

    mCloseButton = new QPushButton("Close", central);
    mainLayout->addWidget(mCloseButton);
    connect(mCloseButton, &QPushButton::clicked, this, &ui::AtomManager::onCloseButtonClicked);

    setCentralWidget(central);
    const int kInitW = 500;            
    const int kInitH = 350;
    resize(kInitW, kInitH);             
    mAtomVTKPreview->setMinimumSize(800, 600);   

}

void ui::AtomManager::onCloseButtonClicked()
{
    parentMDVisualiser->getVTKWidget()->resetCameraToSystem();
    hide();
}

void ui::AtomManager::setParentMDVisualiser(MDVisualiser* visualiser)
{
    parentMDVisualiser = visualiser;
}

void ui::AtomManager::onAddStructureButtonClicked()
{
    NewStructureDialog* newStructureDialog = new NewStructureDialog(this);
    if (newStructureDialog->exec() == QDialog::Accepted)
    {
        ui::StructureParams params = newStructureDialog->get_params();
        std::cout << "Structure params: " << params.structureName.toStdString() << std::endl;
        std::cout << "Structure type: " << params.structureType.toStdString() << std::endl;
        std::cout << "Cubes in X: " << params.cubesX << std::endl;
        std::cout << "Cubes in Y: " << params.cubesY << std::endl;
        std::cout << "Cubes in Z: " << params.cubesZ << std::endl;
        std::cout << "Atom type: " << params.atomType.toStdString() << std::endl;
        std::cout << "Atom spacing: " << params.atomSpacing << std::endl;
        std::cout << "Offset X: " << params.offsetX << std::endl;
        std::cout << "Offset Y: " << params.offsetY << std::endl;
        std::cout << "Offset Z: " << params.offsetZ << std::endl;   


        if (params.structureType == "fcc" && params.atomType == "Cu")
        {
            atoms::Structure fcc_structure(params.cubesX, params.cubesY, params.cubesZ, params.structureName.toStdString());
            for (auto atom : fcc_structure.get_atoms())
            {
                atom.x += params.offsetX;
                atom.y += params.offsetY;
                atom.z += params.offsetZ;
            }

            parentMDVisualiser->current_timestep_data->add_structure(fcc_structure);
            parentMDVisualiser->mSharedData->indexes_of_timesteps_edited_by_ui.push_back(parentMDVisualiser->mPlaybackSettings->current_timestep_index);

        }
        mStructureListViewer->setStructureList(&(parentMDVisualiser->current_timestep_data->structure_list));
    }

}


std::vector<atoms::Atom> ui::AtomManager::get_new_atoms()
{
    return new_atoms;
}

void ui::AtomManager::set_new_atoms(std::vector<atoms::Atom> new_atoms)
{
    this->new_atoms = new_atoms;
}

ui::StructureListViewer* ui::AtomManager::get_structure_list_viewer()
{
    return mStructureListViewer;
}

ui::AtomVTKPreview* ui::AtomManager::get_atom_vtk_preview()
{
    return mAtomVTKPreview;
}


ui::MDVisualiser* ui::AtomManager::get_parent_md_visualiser()
{
    return parentMDVisualiser;
}



