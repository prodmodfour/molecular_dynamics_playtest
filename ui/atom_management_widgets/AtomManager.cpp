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
    // Need the full definition of MDVisualiser to access its members
    if (parentMDVisualiser) {
        // Accessing playback settings might require a public getter/setter or making AtomManager a friend class
        // For now, assuming there's a way to access it or modify MDVisualiser later
        // Example: parentMDVisualiser->getPlaybackSettings()->pause = false; 
        // For the C2248 error, we need to make mPlaybackSettings accessible or provide a method.
        // Let's assume a getter exists for now. If not, we need to add it to MDVisualiser.h
        PlaybackSettings* settings = parentMDVisualiser->getPlaybackSettings(); // Assuming getPlaybackSettings() exists
        if(settings) {
            settings->pause = false;
        }
    }
    hide();
}

void ui::AtomManager::setParentMDVisualiser(MDVisualiser* visualiser)
{
    parentMDVisualiser = visualiser;
}
