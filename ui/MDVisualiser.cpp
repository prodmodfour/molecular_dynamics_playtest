#include "MDVisualiser.h"
#include "../simulation/Timestep.h"
#include <stdexcept>
#include "data_loaders/BasicDataLoader.h"
#include "atom_management_widgets/AtomManager.h"


// Qt includes
#include <QTimer>
#include <QSlider>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QApplication>
#include <QObject>
#include <QSurfaceFormat>
#include <QLineEdit>
#include <QDialog>
#include <iostream>



ui::MDVisualiser::MDVisualiser(
                       QWidget* parent,
                       ui::BasicDataLoader* data_loader,
                       ui::PlaybackSettings* playback_settings)
    : QMainWindow(parent)
    , mDataLoader(data_loader)
    , mPlaybackSettings(playback_settings)
    , current_timestep_data(new simulation::Timestep()) 
{

    setDataLoader(data_loader);


    // Create a central widget and main layout
    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // Create our Atom VTK Widget
    mVTKWidget = new AtomVTKWidget(central);
    mVTKWidget->setFixedSize(1280, 720);
    mainLayout->addWidget(mVTKWidget);


    // --- Playback Controls ---
    QHBoxLayout* controlsLayout = new QHBoxLayout;

    // Speed controls
    mSpeedDownButton = new QPushButton("-", central);
    controlsLayout->addWidget(mSpeedDownButton);

    mSpeedLineEdit = new QLineEdit(central);
    mSpeedLineEdit->setText(QString::number(mPlaybackSettings->speed));
    mSpeedLineEdit->setFixedWidth(50); 
    controlsLayout->addWidget(mSpeedLineEdit);

    mSpeedUpButton = new QPushButton("+", central);
    controlsLayout->addWidget(mSpeedUpButton);


    mStartPauseButton = new QPushButton("Start/Pause", central);
    controlsLayout->addWidget(mStartPauseButton);

    mDisplayedTimestepLineEdit = new QLineEdit(central);
    mDisplayedTimestepLineEdit->setText(QString::number(mPlaybackSettings->current_timestep_index));
    controlsLayout->addWidget(mDisplayedTimestepLineEdit);


    mReverseButton = new QPushButton("Reverse", central);
    controlsLayout->addWidget(mReverseButton);


    mRestartButton = new QPushButton("Restart", central);
    controlsLayout->addWidget(mRestartButton);


    mManageAtomsButton = new QPushButton("Manage Atoms", central); 
    controlsLayout->addWidget(mManageAtomsButton);
    connect(mManageAtomsButton, &QPushButton::clicked,
            this, &ui::MDVisualiser::onManageAtomsClicked);

    connect(mRestartButton, &QPushButton::clicked,
            this, &ui::MDVisualiser::onRestartClicked);

    mainLayout->addLayout(controlsLayout);
    setCentralWidget(central);

    connect(mSpeedDownButton, &QPushButton::clicked,
            this, &MDVisualiser::onSpeedDownClicked);
    connect(mSpeedUpButton, &QPushButton::clicked,
            this, &MDVisualiser::onSpeedUpClicked);
    connect(mSpeedLineEdit, &QLineEdit::editingFinished,
            this, &MDVisualiser::onSpeedLineEditChanged);


    connect(mStartPauseButton, &QPushButton::clicked,
            this, &ui::MDVisualiser::onStartPauseClicked);


    connect(mReverseButton, &QPushButton::clicked,
            this, &ui::MDVisualiser::onReverseClicked);

    // QT timer that updates the animation
    mTimer = new QTimer(this);
    connect(mTimer, &QTimer::timeout,
            this, &ui::MDVisualiser::onTimerTimeout);
    mTimer->start(42); // update every 42 ms (approx. 24 FPS)

    mAtomManager = new ui::AtomManager(this);
    mAtomManager->setParentMDVisualiser(this);

}

void ui::MDVisualiser::onTimerTimeout()
{
    if (mPlaybackSettings->pause == false)
    {
        mDataLoader->updateLastTimestepIndex();
        mPlaybackSettings->next_timestep();
        updateDisplayedTimestepLineEdit();
    }

    if (mDataLoader && mDataLoader->load()) 
    {
        if (current_timestep_data) {
             mVTKWidget->updateAtoms(current_timestep_data->atoms);
        } else {
            std::cerr << "Error: current_timestep_data is null in onTimerTimeout." << std::endl;
        }
    }

}

void ui::MDVisualiser::onSpeedDownClicked()
{
    bool ok;
    int currentSpeed = mSpeedLineEdit->text().toInt(&ok);
    if (!ok) return; 
    if (currentSpeed > 1)
    {
        currentSpeed--;
        mSpeedLineEdit->setText(QString::number(currentSpeed));
        mPlaybackSettings->change_speed(currentSpeed);
    }
}

void ui::MDVisualiser::onSpeedUpClicked()
{
    bool ok;
    int currentSpeed = mSpeedLineEdit->text().toInt(&ok);
    if (!ok) return; 

    currentSpeed++;
    mSpeedLineEdit->setText(QString::number(currentSpeed));
    mPlaybackSettings->change_speed(currentSpeed);
}

void ui::MDVisualiser::onSpeedLineEditChanged()
{
    bool ok;
    int newSpeed = mSpeedLineEdit->text().toInt(&ok);
    if (!ok || newSpeed <= 0) { 
         mSpeedLineEdit->setText(QString::number(mPlaybackSettings->speed)); 
         return;
    }

    mPlaybackSettings->change_speed(newSpeed);
}

void ui::MDVisualiser::onStartPauseClicked()
{
    mPlaybackSettings->toggle_pause();
}

void ui::MDVisualiser::onReverseClicked()
{
    mPlaybackSettings->change_direction();
}

void ui::MDVisualiser::onRestartClicked()
{

    qApp->exit(1);
}


void ui::MDVisualiser::setDataLoader(ui::BasicDataLoader* data_loader)
{
    mDataLoader = data_loader;
    if (mDataLoader) {
        mDataLoader->setPlaybackSettings(mPlaybackSettings);
        mDataLoader->setVisualiser(this);
    } else {
         std::cerr << "Error: Attempted to set a null data loader." << std::endl;
    }
}

void ui::MDVisualiser::setPlaybackSettings(ui::PlaybackSettings* playback_settings)
{
    mPlaybackSettings = playback_settings;
}

void ui::MDVisualiser::onDisplayedTimestepLineEditChanged()
{
    mPlaybackSettings->current_timestep_index = mDisplayedTimestepLineEdit->text().toInt();
}

void ui::MDVisualiser::onManageAtomsClicked()
{
    mPlaybackSettings->pause = true;

    mAtomManager->show();
}

void ui::MDVisualiser::updateDisplayedTimestepLineEdit()
{
    mDisplayedTimestepLineEdit->setText(QString::number(mPlaybackSettings->current_timestep_index));
}

void ui::MDVisualiser::setSharedData(SharedData* shared_data)
{
    mSharedData = shared_data;
}

