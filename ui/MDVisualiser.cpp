#include "MDVisualiser.h"
#include "../simulation/Timestep.h"
#include "../ui/data_loaders/BasicDataLoader.h"


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
#include <stdexcept>
#include "data_loaders/BasicDataLoader.h"



ui::MDVisualiser::MDVisualiser(
                       QWidget* parent,
                       ui::BasicDataLoader* data_loader,
                       ui::PlaybackSettings* playback_settings)
    : QMainWindow(parent)
    , data_loader_set(false)
    , mDataLoader(data_loader)
    , mPlaybackSettings(playback_settings)
    , current_timestep_data(new simulation::Timestep())
{

    setDataLoader(data_loader);
    // Give the data output pointer to the data loader
    mDataLoader->setDataOutputPointer(current_timestep_data);

    // Create a central widget and main layout
    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // Create our Atom VTK Widget
    mVTKWidget = new AtomVTKWidget(central);
    mVTKWidget->setFixedSize(1280, 720);
    mainLayout->addWidget(mVTKWidget);


    // --- Playback Controls ---
    QHBoxLayout* controlsLayout = new QHBoxLayout;

    // Speed slider
    mSpeedSlider = new QSlider(Qt::Horizontal, central);
    mSpeedSlider->setRange(1, 10);
    mSpeedSlider->setValue(1); // default speed
    controlsLayout->addWidget(mSpeedSlider);

    // Start/Pause button
    mStartPauseButton = new QPushButton("Start/Pause", central);
    controlsLayout->addWidget(mStartPauseButton);

    // Reverse button
    mReverseButton = new QPushButton("Reverse", central);
    controlsLayout->addWidget(mReverseButton);

    // Restart button
    mRestartButton = new QPushButton("Restart", central);
    controlsLayout->addWidget(mRestartButton);

    connect(mRestartButton, &QPushButton::clicked,
            this, &ui::MDVisualiser::onRestartClicked);

    mainLayout->addLayout(controlsLayout);
    setCentralWidget(central);

    // Connect slider to playback manager
    connect(mSpeedSlider, &QSlider::valueChanged,
            this, &ui::MDVisualiser::onSpeedChanged);

    // Connect start/pause button
    connect(mStartPauseButton, &QPushButton::clicked,
            this, &ui::MDVisualiser::onStartPauseClicked);

    // Connect reverse button
    connect(mReverseButton, &QPushButton::clicked,
            this, &ui::MDVisualiser::onReverseClicked);

    // Setup timer to update animation
    mTimer = new QTimer(this);
    connect(mTimer, &QTimer::timeout,
            this, &ui::MDVisualiser::onTimerTimeout);
    mTimer->start(42); // update every 42 ms (approx. 24 FPS)
    
    

}

void ui::MDVisualiser::onTimerTimeout()
{
    if (mPlaybackSettings->pause == false)
    {
        mPlaybackSettings->next_timestep();

        mDataLoader->load();
    }


    // // // Render the new timestep
    mVTKWidget->updateAtoms(current_timestep_data->atoms);

}

void ui::MDVisualiser::onSpeedChanged(int value)
{
    mPlaybackSettings->change_speed(value);
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
    // Signal the application event loop to exit with code 1:
    qApp->exit(1);
}



void ui::MDVisualiser::setDataLoader(ui::BasicDataLoader* data_loader)
{
    mDataLoader = data_loader;
    mDataLoader->setPlaybackSettings(mPlaybackSettings);
    mDataLoader->setDataOutputPointer(current_timestep_data);
    mDataLoader->setVisualiser(this);
    data_loader_set = true;
}

void ui::MDVisualiser::setPlaybackSettings(ui::PlaybackSettings* playback_settings)
{
    mPlaybackSettings = playback_settings;
}



