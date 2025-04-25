#include "AtomManager.h"
#include "../MDVisualiser.h" 

#include <QVBoxLayout>
#include <QPushButton>


ui::AtomManager::AtomManager(QWidget* parent) : QMainWindow(parent)
{
    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    mCloseButton = new QPushButton("Close", central);
    mainLayout->addWidget(mCloseButton);

    setCentralWidget(central);
    connect(mCloseButton, &QPushButton::clicked, this, &ui::AtomManager::onCloseButtonClicked);

}

void ui::AtomManager::onCloseButtonClicked()
{
    parentMDVisualiser->getPlaybackSettings()->pause = false;
    parentMDVisualiser->getVTKWidget()->resetCameraToSystem();
    hide();
}

void ui::AtomManager::setParentMDVisualiser(MDVisualiser* visualiser)
{
    parentMDVisualiser = visualiser;
}
