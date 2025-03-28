#pragma once

#include <QMainWindow>
#include <vector>

#include "AtomVTKWidget.h"
#include "PlaybackSettings.h"
#include "../simulation/Timestep.h"


class QTimer;
class QSlider;
class QPushButton;

namespace ui
{

class BasicDataLoader;
class MDVisualiser : public QMainWindow
{
    Q_OBJECT

public:
    simulation::Timestep*      current_timestep_data;
    explicit MDVisualiser(
                        QWidget* parent = nullptr,
                        ui::BasicDataLoader* data_loader = nullptr,
                        ui::PlaybackSettings* playback_settings = nullptr);

public slots:
    void onTimerTimeout();
    void onSpeedChanged(int value);
    void onStartPauseClicked();
    void onReverseClicked();
    void onRestartClicked();
    void setPlaybackSettings(ui::PlaybackSettings* playback_settings);
    void setDataLoader(ui::BasicDataLoader* data_loader);

private:
    ui::AtomVTKWidget*            mVTKWidget;
    QSlider*                  mSpeedSlider;
    QPushButton*              mStartPauseButton;
    QPushButton*              mReverseButton;
    QPushButton*              mRestartButton;
    QTimer*                   mTimer;

    
    ui::PlaybackSettings*       mPlaybackSettings;
    bool                        data_loader_set;
    ui::BasicDataLoader*        mDataLoader;
};
    
} // namespace ui