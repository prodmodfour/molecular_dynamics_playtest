#pragma once

#include <QMainWindow>
#include <vector>

#include "AtomVTKWidget.h"
#include "PlaybackSettings.h"
#include "../simulation/Timestep.h"
#include "../simulation/SimulationTimeline.h"
#include "AtomPropertiesWidget.h"


class QTimer;
class QSlider;
class QPushButton;

namespace ui
{
class MDVisualiser : public QMainWindow
{
    Q_OBJECT

public:
    explicit MDVisualiser(simulation::SimulationTimeline &simulation_timeline,
                        QWidget* parent = nullptr);

private slots:
    void onTimerTimeout();
    void onSpeedChanged(int value);
    void onStartPauseClicked();
    void onReverseClicked();
    void onRestartClicked();

private:
    ui::AtomVTKWidget*            mVTKWidget;
    ui::AtomPropertiesWidget*     mAtomPropertiesWidget;
    QSlider*                  mSpeedSlider;
    QPushButton*              mStartPauseButton;
    QPushButton*              mReverseButton;
    QPushButton*              mRestartButton;
    QTimer*                   mTimer;

    simulation::SimulationTimeline      mSimulationTimeline;
    ui::PlaybackSettings           mPlaybackSettings;
};
    
} // namespace ui