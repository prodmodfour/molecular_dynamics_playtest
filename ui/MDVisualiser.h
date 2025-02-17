#pragma once

#include <QMainWindow>
#include <vector>

#include "AtomVTKWidget.h"
#include "PlaybackManager.h"
#include "../simulation/Timestep.h"

namespace ui {
class QTimer;
class QSlider;
class QPushButton;

class MDVisualiser : public QMainWindow
{
    Q_OBJECT

public:
    explicit MDVisualiser(const std::vector<Timestep>& simulation_data,
                        QWidget* parent = nullptr);

private slots:
    void onTimerTimeout();
    void onSpeedChanged(int value);
    void onStartPauseClicked();
    void onReverseClicked();
    void onRestartClicked();

private:
    AtomVTKWidget*            mVTKWidget;
    QSlider*                  mSpeedSlider;
    QPushButton*              mStartPauseButton;
    QPushButton*              mReverseButton;
    QPushButton*              mRestartButton;
    QTimer*                   mTimer;

    std::vector<Timestep>     mSimulationData;
    PlaybackManager           mPlaybackManager;
};

}