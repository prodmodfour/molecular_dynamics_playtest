#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QSlider>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QApplication>
#include <QObject>

#include "AtomVTKWidget.h"
#include "PlaybackManager.h"
#include "../simulation/Timestep.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const std::vector<Timestep>& simulation_data, QWidget* parent = nullptr)
        : QMainWindow(parent)
        , mSimulationData(simulation_data)
        , mPlaybackManager(simulation_data.size())
    {
        // Central widget and layout
        QWidget* central = new QWidget(this);
        QVBoxLayout* mainLayout = new QVBoxLayout(central);

        // Create our Atom VTK Widget
        mVTKWidget = new AtomVTKWidget(central);
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

        mRestartButton = new QPushButton("Restart", central);
        controlsLayout->addWidget(mRestartButton);

        connect(mRestartButton, &QPushButton::clicked,
                this, &MainWindow::onRestartClicked);

        mainLayout->addLayout(controlsLayout);
        setCentralWidget(central);

        // Connect slider to playback manager
        connect(mSpeedSlider, &QSlider::valueChanged,
                this, &MainWindow::onSpeedChanged);

        // Connect start/pause button
        connect(mStartPauseButton, &QPushButton::clicked,
                this, &MainWindow::onStartPauseClicked);

        // Connect reverse button
        connect(mReverseButton, &QPushButton::clicked,
                this, &MainWindow::onReverseClicked);

        // Setup timer to update animation
        mTimer = new QTimer(this);
        connect(mTimer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
        mTimer->start(42); // update every 42 ms, roughly 24 fps

        // Initialize with the first timestep (if available)
        if (!mSimulationData.empty())
            mVTKWidget->updateAtoms(mSimulationData[mPlaybackManager.current_timestep].atoms);
    }

private slots:
    void onTimerTimeout()
    {
        // Update the current timestep in the playback manager
        mPlaybackManager.update_timestep();

        // Render the new timestep
        int current = mPlaybackManager.current_timestep;
        if (current >= 0 && current < (int)mSimulationData.size())
        {
            mVTKWidget->updateAtoms(mSimulationData[current].atoms);
        }
    }

    void onSpeedChanged(int value)
    {
        mPlaybackManager.change_speed(value);
    }

    void onStartPauseClicked()
    {
        mPlaybackManager.toggle_pause();
    }

    void onReverseClicked()
    {
        mPlaybackManager.change_direction();
    }

    void onRestartClicked()
    {
        // Signal the application event loop to exit with code 1:
        qApp->exit(1);
    }

private:
    AtomVTKWidget* mVTKWidget;
    QSlider* mSpeedSlider;
    QPushButton* mStartPauseButton;
    QPushButton* mReverseButton;
    QPushButton* mRestartButton;
    QTimer* mTimer;

    std::vector<Timestep> mSimulationData;
    PlaybackManager mPlaybackManager;
};
