#include "MDVisualiser.h"
#include "../simulation/Timestep.h"
#include <stdexcept>
#include "data_loaders/BasicDataLoader.h"
#include "atom_management_widgets/AtomManager.h"


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
#include <QLineEdit>
#include <QDialog>
#include <QToolButton>
#include <QDoubleSpinBox>
#include <QLCDNumber>
#include <QSizePolicy>
#include <QStyle>
#include <QLabel>


#include <iostream>



ui::MDVisualiser::MDVisualiser(
                       QWidget* parent,
                       ui::BasicDataLoader* data_loader,
                       ui::PlaybackSettings* playback_settings)
    : QMainWindow(parent)
    , mDataLoader(data_loader)
    , mPlaybackSettings(playback_settings)
    , current_timestep_data(new simulation::Timestep()) 
{

    setDataLoader(data_loader);



    // QT timer that updates the animation
    mTimer = new QTimer(this);
    connect(mTimer, &QTimer::timeout,
            this, &ui::MDVisualiser::onTimerTimeout);
    mTimer->start(42); // update every 42 ms (approx. 24 FPS)




    // ---------------  UI SET-UP  ---------------
    // Central container
    auto *central = new QWidget(this);
    auto *main   = new QVBoxLayout(central);
    main->setContentsMargins(6,6,6,6);          
    main->setSpacing(6);

    // ---------- 1) VTK ----------
    mVTKWidget = new AtomVTKWidget(central);
    mVTKWidget->setSizePolicy(QSizePolicy::Expanding,
                            QSizePolicy::Expanding); 
    main->addWidget(mVTKWidget, 1);

    // ---------- 2) Playback toolbar ----------
    auto *bar = new QGridLayout;          
    bar->setHorizontalSpacing(8);
    bar->setVerticalSpacing(2);

    // White space
    bar->addItem(new QSpacerItem(0, 0,
                QSizePolicy::Expanding, QSizePolicy::Minimum), 0, 0);

    // speed ↓
    auto down = new QToolButton(central);
    down->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    bar->addWidget(down, 0, 1, Qt::AlignVCenter);

    // speed value (line-edit-style spin box)
    auto speedSpin = new QDoubleSpinBox(central);
    speedSpin->setRange(0.1, 20.0);
    speedSpin->setSingleStep(0.1);
    speedSpin->setValue(mPlaybackSettings->speed);
    speedSpin->setSuffix(u8"×");
    speedSpin->setFixedWidth(70);
    mSpeedSpin = speedSpin;
    bar->addWidget(speedSpin, 0, 2, Qt::AlignVCenter);   

    // “Speed” label directly underneath
    auto speedLabel = new QLabel(tr("Speed"), central);
    speedLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    QFont f = speedLabel->font();
    f.setPointSizeF(f.pointSizeF() - 1);
    speedLabel->setFont(f);
    speedLabel->setStyleSheet("color:#666;");
    bar->addWidget(speedLabel, 1, 2, Qt::AlignHCenter); 

    // speed ↑
    auto up = new QToolButton(central);
    up->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    bar->addWidget(up, 0, 3, Qt::AlignVCenter);

    // play / pause
    auto playPause = new QToolButton(central);
    playPause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    playPause->setCheckable(true);
    bar->addWidget(playPause, 0, 4, Qt::AlignVCenter);

    // current step (read-only LCD)
    auto stepLcd = new QLCDNumber(4, central);
    stepLcd->display(mPlaybackSettings->current_timestep_index);
    stepLcd->setSegmentStyle(QLCDNumber::Flat);
    stepLcd->setMinimumWidth(30);
    mStepLcd = stepLcd;
    bar->addWidget(stepLcd, 0, 5, Qt::AlignVCenter);

    // reverse
    auto reverse = new QToolButton(central);
    reverse->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    bar->addWidget(reverse, 0, 6, Qt::AlignVCenter);

    // restart
    auto restart = new QToolButton(central);
    restart->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    bar->addWidget(restart, 0, 7, Qt::AlignVCenter);

    // White space
    bar->addItem(new QSpacerItem(0, 0,
                QSizePolicy::Expanding, QSizePolicy::Minimum), 0, 8);


    main->addLayout(bar);


    // ---------- 3) Secondary controls ----------
    auto *footer = new QHBoxLayout;
    footer->addStretch();
    mAtomManager = new AtomManager(central);
    mAtomManager->setParentMDVisualiser(this);
    auto manageAtoms = new QPushButton("Manage atoms…", central);
    footer->addWidget(manageAtoms);
    main->addLayout(footer);


    setCentralWidget(central);
    const int kInitW = 1000;            
    const int kInitH = 700;
    resize(kInitW, kInitH);             
    mVTKWidget->setMinimumSize(800, 600);   

    // ---------------  SIGNALS  ---------------
    connect(down,        &QToolButton::clicked, this, &MDVisualiser::onSpeedDownClicked);
    connect(up,          &QToolButton::clicked, this, &MDVisualiser::onSpeedUpClicked);
    connect(speedSpin,   QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                        this, &MDVisualiser::onSpeedChanged);
    connect(playPause,   &QToolButton::toggled, this, &MDVisualiser::onStartPauseToggled);
    connect(reverse,     &QToolButton::clicked, this, &MDVisualiser::onReverseClicked);
    connect(restart,     &QToolButton::clicked, this, &MDVisualiser::onRestartClicked);
    connect(manageAtoms, &QPushButton::clicked, this, &MDVisualiser::onManageAtomsClicked);

    // ---------------  STYLE  ---------------
    qApp->setStyleSheet(R"(
        QWidget      { font-family: 'Inter', 'Segoe UI', sans-serif; font-size: 10.5pt; }
        QToolButton  { background:#2d2d30; border:1px solid #3e3e42; padding:6px; border-radius:4px; }
        QToolButton:hover         { background:#3e3e42; }
        QToolButton:checked       { background:#5189ff; }
        QToolButton:disabled      { background:#555;        color:#777; }

        QPushButton  { background:#5189ff; color:white; border:0; padding:8px 14px; border-radius:4px; }
        QPushButton:hover         { background:#6ca0ff; }
        QDoubleSpinBox, QLCDNumber{
            background:#1e1e1e;   color:#d6d6d6; border:1px solid #3e3e42; border-radius:4px; }
    )");

}

void ui::MDVisualiser::onTimerTimeout()
{
    if (mPlaybackSettings->pause == false)
    {
        mDataLoader->updateLastTimestepIndex();
        mPlaybackSettings->next_timestep();
        updateStepDisplay();
    }

    if (mDataLoader && mDataLoader->load()) 
    {
        if (current_timestep_data) {
             mVTKWidget->updateAtoms(current_timestep_data->atoms);

             if (!FirstViewDone)
             {
                mVTKWidget->resetCameraToSystem();
                FirstViewDone = true;
             }
             
        } else {
            std::cerr << "Error: current_timestep_data is null in onTimerTimeout." << std::endl;
        }
    }

}

void ui::MDVisualiser::onSpeedDownClicked()
{

    double currentSpeed = mSpeedSpin->value();
    if (currentSpeed > 0.1)
    {
        currentSpeed--;
        mSpeedSpin->setValue(currentSpeed);
        mPlaybackSettings->change_speed(currentSpeed);
    }
}

void ui::MDVisualiser::onSpeedUpClicked()
{
    double currentSpeed = mSpeedSpin->value();
    if (currentSpeed < 20)
    {
        currentSpeed++;
        mSpeedSpin->setValue(currentSpeed);
        mPlaybackSettings->change_speed(currentSpeed);
    }
}


void ui::MDVisualiser::onSpeedChanged(double)
{
    double newSpeed = mSpeedSpin->value();
    if (newSpeed <= 0) { 
         mSpeedSpin->setValue(mPlaybackSettings->speed); 
         return;
    }

    mPlaybackSettings->change_speed(newSpeed);
}

void ui::MDVisualiser::onStartPauseToggled(bool)
 {
     if (mPlaybackSettings) mPlaybackSettings->toggle_pause();
 }

void ui::MDVisualiser::onReverseClicked()
{
    mPlaybackSettings->change_direction();
}

void ui::MDVisualiser::onRestartClicked()
{
    mPlaybackSettings->reset();
}


void ui::MDVisualiser::setDataLoader(ui::BasicDataLoader* data_loader)
{
    mDataLoader = data_loader;
    if (mDataLoader) {
        mDataLoader->setPlaybackSettings(mPlaybackSettings);
        mDataLoader->setVisualiser(this);
    } else {
         std::cerr << "Error: Attempted to set a null data loader." << std::endl;
    }
}

void ui::MDVisualiser::setPlaybackSettings(ui::PlaybackSettings* playback_settings)
{
    mPlaybackSettings = playback_settings;
}


void ui::MDVisualiser::onManageAtomsClicked()
{
    mPlaybackSettings->pause = true;

    mAtomManager->show();
}



void ui::MDVisualiser::updateStepDisplay()
{
    if (mStepLcd && mPlaybackSettings)
        mStepLcd->display(mPlaybackSettings->current_timestep_index);
}

void ui::MDVisualiser::setSharedData(SharedData* shared_data)
{
    mSharedData = shared_data;
}

