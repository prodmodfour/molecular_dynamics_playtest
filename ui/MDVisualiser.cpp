#include "MDVisualiser.h"
#include "../simulation/Timestep.h"
#include "../simulation/SimulationTimeline.h"

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



ui::MDVisualiser::MDVisualiser(simulation::SimulationTimeline &simulation_timeline,
                       QWidget* parent)
    : QMainWindow(parent)
    , mSimulationTimeline(simulation_timeline)
    , mPlaybackSettings()
{

    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
    // Create a central widget and main layout
    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // Create our Atom VTK Widget
    mVTKWidget = new AtomVTKWidget(central);
    mainLayout->addWidget(mVTKWidget);

    mAtomPropertiesWidget = new AtomPropertiesWidget(central);
    mainLayout->addWidget(mAtomPropertiesWidget);

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

    // Initialize with the first timestep (if available)
    if (!mSimulationData.empty())
    {
        mVTKWidget->updateAtoms(mSimulationData[mPlaybackManager.current_timestep].atoms);
    }
}

void ui::MDVisualiser::onTimerTimeout()
{

    if (mPlaybackSettings.pause)
    {
        mVTKWidget->render();
        return;
    }

    if (mPlaybackSettings.direction == 1)
    {
        mSimulationTimeline.progress();
    }
    else if (mPlaybackSettings.direction == -1)
    {
        mSimulationTimeline.backtrack();
    }
    else
    {
        throw std::invalid_argument("Invalid direction");
    }

    // Render the new timestep
    mVTKWidget->updateAtoms(mSimulationTimeline.current_timestep.atoms);

}

void ui::MDVisualiser::onSpeedChanged(int value)
{
    mPlaybackSettings.change_speed(value);
}

void ui::MDVisualiser::onStartPauseClicked()
{
    mPlaybackSettings.toggle_pause();
}

void ui::MDVisualiser::onReverseClicked()
{
    mPlaybackSettings.change_direction();
}

void ui::MDVisualiser::onRestartClicked()
{
    // Signal the application event loop to exit with code 1:
    qApp->exit(1);
}


