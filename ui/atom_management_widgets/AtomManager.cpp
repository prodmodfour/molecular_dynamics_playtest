#include "AtomManager.h"

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
    parentMDVisualiser->mPlaybackSettings->pause = false;
    hide();
}


