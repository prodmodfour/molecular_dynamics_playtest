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
                        ui::PlaybackSettings* playback_settings = nullptr
                        // ui::SharedData* shared_data = nullptr,
                        // ui::SharedData local_unsaved_changes_to_shared_data = SharedData()
                        );

public slots:
    void onTimerTimeout();
    void onSpeedLineEditChanged();
    void onSpeedUpClicked();
    void onSpeedDownClicked();
    void onStartPauseClicked();
    void onReverseClicked();
    void onRestartClicked();
    void setPlaybackSettings(ui::PlaybackSettings* playback_settings);
    void setDataLoader(ui::BasicDataLoader* data_loader);

private:
    ui::AtomVTKWidget*            mVTKWidget;
    QPushButton* mSpeedDownButton; 
    QLineEdit* mSpeedLineEdit;
    QPushButton* mSpeedUpButton;
    QPushButton*              mStartPauseButton;
    QPushButton*              mReverseButton;
    QPushButton*              mRestartButton;
    QTimer*                   mTimer;

    ui::PlaybackSettings*       mPlaybackSettings;
    ui::BasicDataLoader*        mDataLoader;
};
    
} // namespace ui