#include "AtomManager.h"
#include "../MDVisualiser.h" 
#include "AtomVTKPreview.h"
#include "StructureListViewer.h"
#include "../../simulation/StructureList.h"
#include <QVBoxLayout>
#include <QPushButton>
#include "../../atoms/Atom.h"
#include <vector>


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

    mCloseButton = new QPushButton("Close", central);
    mainLayout->addWidget(mCloseButton);

    setCentralWidget(central);
    const int kInitW = 500;            
    const int kInitH = 350;
    resize(kInitW, kInitH);             
    mAtomVTKPreview->setMinimumSize(400, 300);   
    connect(mCloseButton, &QPushButton::clicked, this, &ui::AtomManager::onCloseButtonClicked);

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
