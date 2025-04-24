#pragma once

#include <QMainWindow>
#include <vector>

#include "AtomVTKWidget.h"
#include "PlaybackSettings.h"
#include "../simulation/Timestep.h"
#include "SharedData.h"
#include "atom_management_widgets/AtomManager.h"
class QTimer;
class QSlider;
class QPushButton;
class QLineEdit;
class QAction;
class QMenu;

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
                        // SharedData* shared_data = nullptr,
                        //SharedData local_unsaved_changes_to_shared_data = SharedData()
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
    void setSharedData(SharedData* shared_data);
    void onManageAtomsClicked();
    void onDisplayedTimestepLineEditChanged();

private:
    void updateDisplayedTimestepLineEdit(); 
    ui::AtomVTKWidget*            mVTKWidget;
    ui::AtomManager*              mAtomManager;
    QPushButton* mSpeedDownButton; 
    QLineEdit* mSpeedLineEdit;
    QLineEdit* mDisplayedTimestepLineEdit;
    QPushButton* mSpeedUpButton;
    QPushButton*              mStartPauseButton;
    QPushButton*              mReverseButton;
    QPushButton*              mRestartButton;
    QPushButton*              mManageAtomsButton;
    QTimer*                   mTimer;

    ui::PlaybackSettings*       mPlaybackSettings;
    ui::BasicDataLoader*        mDataLoader;
    SharedData*                 mSharedData;
};
    
} // namespace ui