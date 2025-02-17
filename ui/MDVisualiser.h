#pragma once

#include <QMainWindow>
#include <vector>

#include "AtomVTKWidget.h"
#include "PlaybackManager.h"
#include "../simulation/Timestep.h"


class QTimer;
class QSlider;
class QPushButton;

namespace ui
{
class MDVisualiser : public QMainWindow
{
    Q_OBJECT

public:
    explicit MDVisualiser(const std::vector<simulation::Timestep>& simulation_data,
                        QWidget* parent = nullptr);

private slots:
    void onTimerTimeout();
    void onSpeedChanged(int value);
    void onStartPauseClicked();
    void onReverseClicked();
    void onRestartClicked();

private:
    ui::AtomVTKWidget*            mVTKWidget;
    QSlider*                  mSpeedSlider;
    QPushButton*              mStartPauseButton;
    QPushButton*              mReverseButton;
    QPushButton*              mRestartButton;
    QTimer*                   mTimer;

    std::vector<simulation::Timestep>     mSimulationData;
    ui::PlaybackManager           mPlaybackManager;
};
    
} // namespace ui