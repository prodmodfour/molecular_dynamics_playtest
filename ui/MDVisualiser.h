#pragma once

#include <QMainWindow>
#include <vector>

#include "AtomVTKWidget.h"
#include "PlaybackSettings.h"
#include "../simulation/Timestep.h"
#include "SharedData.h"

class QTimer;
class QSlider;
class QPushButton;
class QLineEdit;
class QAction;
class QMenu;
class QToolButton;
class QSpinBox;
class QLCDNumber;
class QLabel;
class QMenuBar;
class QMenu;
class QActionGroup;



namespace ui {
    class AtomManager;
    class BasicDataLoader;
    class SimulationSettingsDialogue;   
}

namespace ui
{


class MDVisualiser : public QMainWindow
{
    Q_OBJECT

public:
    explicit MDVisualiser(QWidget*              parent            = nullptr,
                          ui::BasicDataLoader*  data_loader       = nullptr,
                          ui::PlaybackSettings* playback_settings = nullptr,
                          SharedData*           shared_data       = nullptr);

    simulation::Timestep* current_timestep_data = nullptr;


    ui::AtomManager*      getAtomManager()      { return mAtomManager; }
    ui::PlaybackSettings* getPlaybackSettings() { return mPlaybackSettings; }

public slots:
    ui::AtomVTKWidget* getVTKWidget() { return mVTKWidget; }


    void onTimerTimeout();


    void onSpeedChanged(int value);        
    void onSpeedUpClicked();
    void onSpeedDownClicked();

 
    void onStartPauseToggled(bool playing);     
    void onReverseClicked();
    void onRestartClicked();

 
    void onManageAtomsClicked();
    void onSimulationSettingsClicked();
    void onClearAtomsClicked();
    void onAddAtomClicked();

    void onParallelCameraToggled(bool checked);
    void onPerspectiveCameraToggled(bool checked);
    void onResetCameraClicked();

 
    void setPlaybackSettings(ui::PlaybackSettings* playback_settings);
    void setDataLoader(ui::BasicDataLoader* data_loader);
    void setSharedData(SharedData* shared_data);

private:
    void updateStepDisplay();     


    ui::AtomVTKWidget* mVTKWidget      = nullptr;
    ui::AtomManager*   mAtomManager    = nullptr;

    QToolButton*       mSpeedDownBtn   = nullptr;
    QSpinBox*          mSpeedSpin      = nullptr;
    QToolButton*       mSpeedUpBtn     = nullptr;

    QToolButton*       mPlayPauseBtn   = nullptr;
    QLCDNumber*        mStepLcd        = nullptr;
    QToolButton*       mReverseBtn     = nullptr;
    QToolButton*       mRestartBtn     = nullptr;
    QPushButton*       mManageAtomsBtn = nullptr;   
    bool               FirstViewDone   = false;

    QTimer*            mTimer          = nullptr;

    QMenuBar*         mMenuBar        = nullptr;


    ui::PlaybackSettings* mPlaybackSettings = nullptr;
    ui::BasicDataLoader*  mDataLoader       = nullptr;
    SharedData*           mSharedData       = nullptr;
};

} // namespace ui