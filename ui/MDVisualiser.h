#pragma once

#include <QMainWindow>
#include <vector>

#include "AtomVTKWidget.h"
#include "PlaybackSettings.h"
#include "../simulation/Timestep.h"
#include "../simulation/SimulationTimeline.h"
#include "AtomPropertiesWidget.h"
#include "data_loaders/BasicDataLoader.h"

class QTimer;
class QSlider;
class QPushButton;

namespace ui
{
class MDVisualiser : public QMainWindow
{
    Q_OBJECT

public:
    explicit MDVisualiser(
                        QWidget* parent = nullptr);

private slots:
    void onTimerTimeout();
    void onSpeedChanged(int value);
    void onStartPauseClicked();
    void onReverseClicked();
    void onRestartClicked();
    void setPlaybackSettings(ui::PlaybackSettings* playback_settings);
    void setDataLoader(ui::BasicDataLoader* data_loader);

private:
    ui::AtomVTKWidget*            mVTKWidget;
    ui::AtomPropertiesWidget*     mAtomPropertiesWidget;
    QSlider*                  mSpeedSlider;
    QPushButton*              mStartPauseButton;
    QPushButton*              mReverseButton;
    QPushButton*              mRestartButton;
    QTimer*                   mTimer;

    simulation::Timestep*      current_timestep_data;
    ui::PlaybackSettings*       mPlaybackSettings;
    bool                        data_loader_set;
    ui::BasicDataLoader*        mDataLoader;
};
    
} // namespace ui