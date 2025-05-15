#include "MDVisualiser.h"
#include "../simulation/Timestep.h"
#include <stdexcept>
#include "../data_loaders/BasicDataLoader.h"
#include "../atom_management_widgets/AtomManager.h"
#include <vtkCamera.h>
#include <vtkRendererCollection.h>
#include <vtkRenderer.h>
#include <mutex>
#include "../simulation_management_widgets/SimulationSettingsDialogue.h"
#include "../atoms/Atom.h"
#include <random>
#include "../atom_management_widgets/StructureListViewer.h"
#include "../atom_management_widgets/AtomVTKPreview.h"
#include "../graphs/EnergyGraphWidget.h"
#include "../graphs/KineticEnergyHistogramWidget.h"


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
#include <QSpinBox>
#include <QLCDNumber>
#include <QSizePolicy>
#include <QStyle>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QActionGroup>


#include <iostream>



ui::MDVisualiser::MDVisualiser(
                       QWidget* parent,
                       ui::BasicDataLoader* data_loader,
                       ui::PlaybackSettings* playback_settings,
                       SharedData* shared_data)
    : QMainWindow(parent)
    , mDataLoader(data_loader)
    , mPlaybackSettings(playback_settings)
    , current_timestep_data(new simulation::Timestep()) 
    , mSharedData(shared_data)
{

    setDataLoader(data_loader);

    // update every 42 ms (approx. 24 FPS)
    mTimer = new QTimer(this);
    connect(mTimer, &QTimer::timeout,
            this, &ui::MDVisualiser::onTimerTimeout);
    mTimer->start(42); 


    mEnergyGraphWidget = new graphs::EnergyGraphWidget(this);

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

    // speed down
    auto down = new QToolButton(central);
    down->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    bar->addWidget(down, 0, 1, Qt::AlignVCenter);

    // speed value 
    auto speedSpin = new QSpinBox(central);
    speedSpin->setRange(1, 20);         
    speedSpin->setSingleStep(1);
    speedSpin->setValue(static_cast<int>(mPlaybackSettings->speed));
    speedSpin->setSuffix(u8"×");
    speedSpin->setFixedWidth(70);
    mSpeedSpin = speedSpin;
    bar->addWidget(speedSpin, 0, 2, Qt::AlignVCenter);   

    // “Speed” label
    auto speedLabel = new QLabel(tr("Speed"), central);
    speedLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    QFont f = speedLabel->font();
    f.setPointSizeF(f.pointSizeF() - 1);
    speedLabel->setFont(f);
    speedLabel->setStyleSheet("color:#666;");
    bar->addWidget(speedLabel, 1, 2, Qt::AlignHCenter); 

    // speed up
    auto up = new QToolButton(central);
    up->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    bar->addWidget(up, 0, 3, Qt::AlignVCenter);

    // play / pause
    auto playPause = new QToolButton(central);
    playPause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    playPause->setCheckable(true);
    bar->addWidget(playPause, 0, 4, Qt::AlignVCenter);
    // reverse
    auto reverse = new QToolButton(central);
    reverse->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    bar->addWidget(reverse, 0, 6, Qt::AlignVCenter);



    // White space
    bar->addItem(new QSpacerItem(0, 0,
                QSizePolicy::Expanding, QSizePolicy::Minimum), 0, 8);


    main->addLayout(bar);


    // ---------- 3) Menu bar ----------
    QMenuBar* mMenuBar       = menuBar();
    QMenu* simulationMenu = mMenuBar->addMenu(tr("&Simulation"));
    QMenu* cameraMenu     = mMenuBar->addMenu(tr("&Camera"));
    QMenu* atomsMenu      = mMenuBar->addMenu(tr("&Atoms"));
    QMenu* graphsMenu     = mMenuBar->addMenu(tr("&Graphs"));


    setCentralWidget(central);
    const int kInitW = 1000;            
    const int kInitH = 700;
    resize(kInitW, kInitH);             
    mVTKWidget->setMinimumSize(800, 600);


    // ─── Simulation ───────────────────────────────────────────────────────────────

    QAction* simulationSettingsAct = simulationMenu->addAction(tr("Simulation settings…"));
    connect(simulationSettingsAct, &QAction::triggered, this, &MDVisualiser::onSimulationSettingsClicked);


    QAction* restartAct = simulationMenu->addAction(tr("Restart simulation"));
    connect(restartAct, &QAction::triggered,
            this,       &ui::MDVisualiser::onRestartClicked);


    // ─── Camera ───────────────────────────────────────────────────────────────────

    QAction* resetCameraAct = cameraMenu->addAction(tr("Reset Camera"));
    connect(resetCameraAct, &QAction::triggered, this, &MDVisualiser::onResetCameraClicked);

    QMenu* camModeMenu      = cameraMenu->addMenu(tr("Camera Mode"));
    QActionGroup* camGroup  = new QActionGroup(this);   

    QAction* parallelAct    = camModeMenu->addAction(tr("Parallel"));
    parallelAct->setCheckable(true);
    QAction* perspectiveAct = camModeMenu->addAction(tr("Perspective"));
    perspectiveAct->setCheckable(true);
    perspectiveAct->setChecked(true);              

    camGroup->addAction(parallelAct);
    camGroup->addAction(perspectiveAct);
    connect(parallelAct, &QAction::triggered, this, &MDVisualiser::onParallelCameraToggled);
    connect(perspectiveAct, &QAction::triggered, this, &MDVisualiser::onPerspectiveCameraToggled);


    // ─── Atoms ────────────────────────────────────────────────────────────────────

    QAction* manageAtomsAct = atomsMenu->addAction(tr("Manage Atoms…"));
    mAtomManager = new AtomManager(central);
    mAtomManager->setParentMDVisualiser(this);
    connect(manageAtomsAct, &QAction::triggered,
            this,           &ui::MDVisualiser::onManageAtomsClicked);


    QAction* clearAtomsAct = atomsMenu->addAction(tr("Clear Atoms"));
    connect(clearAtomsAct, &QAction::triggered, this, &MDVisualiser::onClearAtomsClicked);

    // ─── Graphs ───────────────────────────────────────────────────────────────

    QAction* showEnergyLineGraph = graphsMenu->addAction(tr("Energy Line Graph"));
    connect(showEnergyLineGraph, &QAction::triggered, this, &MDVisualiser::onShowEnergyLineGraphClicked);

    QAction* showKineticEnergyHistogram = graphsMenu->addAction(tr("Kinetic Energy Histogram"));
    connect(showKineticEnergyHistogram, &QAction::triggered, this, &MDVisualiser::onShowKineticEnergyHistogramClicked);


    // This exists for testing whether adding atoms works.
    // I'll leave it in for now...
    QAction* addAtomAct = atomsMenu->addAction(tr("Add Random Atom"));
    connect(addAtomAct, &QAction::triggered, this, &MDVisualiser::onAddAtomClicked);

    // ---------------  SIGNALS  ---------------
    connect(down,        &QToolButton::clicked, this, &MDVisualiser::onSpeedDownClicked);
    connect(up,          &QToolButton::clicked, this, &MDVisualiser::onSpeedUpClicked);
    connect(speedSpin,   QOverload<int>::of(&QSpinBox::valueChanged),
                        this, &MDVisualiser::onSpeedChanged);
    connect(playPause,   &QToolButton::toggled, this, &MDVisualiser::onStartPauseToggled);
    connect(reverse,     &QToolButton::clicked, this, &MDVisualiser::onReverseClicked);

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
    // It seems that prints don't work here, possibly because of the way that Qt works.
    if (mPlaybackSettings->pause == false)
    {
        std::unique_lock<std::mutex> lock(mSharedData->mutex);
        mPlaybackSettings->update_last_timestep_index(mSharedData->index_of_latest_timestep_simulated);
        lock.unlock();
        mPlaybackSettings->next_timestep();
        mEnergyGraphWidget->addDataPoint(mPlaybackSettings->current_timestep_index, current_timestep_data->kinetic_energy, current_timestep_data->potential_energy);
    }

    if (mDataLoader && mDataLoader->load()) 
    {
        if (current_timestep_data) {
            mVTKWidget->updateAtoms(current_timestep_data->atoms);
            if ( mSharedData->index_of_latest_timestep_displayed < mPlaybackSettings->current_timestep_index)
            {
            mSharedData->index_of_latest_timestep_displayed = mPlaybackSettings->current_timestep_index;
            }

            if (!FirstViewDone)
            {


                mVTKWidget->resetCameraToSystem();
                mAtomManager->mAtomVTKPreview->setAtomData(&(current_timestep_data->atoms));
                mAtomManager->mStructureListViewer->setStructureList(&(current_timestep_data->structure_list));
                mAtomManager->mStructureListViewer->refreshList();

                FirstViewDone = true;
            }



            if (mPlaybackSettings->pause == false)
            {
                mAtomManager->mAtomVTKPreview->setAtomData(&(current_timestep_data->atoms));
                mAtomManager->mStructureListViewer->setStructureList(&(current_timestep_data->structure_list));
                mAtomManager->mStructureListViewer->refreshList();

            }
 
             
        } else {
            std::cerr << "Error: current_timestep_data is null in onTimerTimeout." << std::endl;
        }
    }

}

void ui::MDVisualiser::onSpeedDownClicked()
{
    int currentSpeed = mSpeedSpin->value();
    if (currentSpeed > 1)
    {
        --currentSpeed;
        mSpeedSpin->setValue(currentSpeed);
        mPlaybackSettings->change_speed(currentSpeed);
    }
}

void ui::MDVisualiser::onSpeedUpClicked()
{
    int currentSpeed = mSpeedSpin->value();
    if (currentSpeed < 20)
    {
        ++currentSpeed;
        mSpeedSpin->setValue(currentSpeed);
        mPlaybackSettings->change_speed(currentSpeed);
    }
}


void ui::MDVisualiser::onSpeedChanged(int)
{
    int newSpeed = mSpeedSpin->value();
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
        mDataLoader->setSharedData(mSharedData);
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

void ui::MDVisualiser::onParallelCameraToggled(bool checked)
{
    auto ren = mVTKWidget->renderWindow()
                        ->GetRenderers()
                        ->GetFirstRenderer();
    if (ren && ren->GetActiveCamera())
    {
        ren->GetActiveCamera()->SetParallelProjection(true);  
    }
}

void ui::MDVisualiser::onPerspectiveCameraToggled(bool checked)
{
    auto ren = mVTKWidget->renderWindow()
                        ->GetRenderers()
                        ->GetFirstRenderer();
    if (ren && ren->GetActiveCamera())
    {
        ren->GetActiveCamera()->SetParallelProjection(false);
    }
}

void ui::MDVisualiser::onResetCameraClicked()
{
    mVTKWidget->resetCameraToSystem();
}

void ui::MDVisualiser::onSimulationSettingsClicked()
{
    mPlaybackSettings->pause = true;

    auto* simulationSettingsDialogue = new SimulationSettingsDialogue(this, mSharedData);
    simulationSettingsDialogue->exec();    
    mPlaybackSettings->reset();
}

void ui::MDVisualiser::onClearAtomsClicked()
{
    mPlaybackSettings->pause = true;
    mPlaybackSettings->reset();
    mDataLoader->clearData();
}

void ui::MDVisualiser::onAddAtomClicked()
{
    mPlaybackSettings->pause = true;
    atoms::Atom atom("Cu", 63.546, 1.28);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 5.0);
    atom.x = dis(gen);
    atom.y = dis(gen);
    atom.z = dis(gen);

    current_timestep_data->atoms.push_back(atom);
    mSharedData->indexes_of_timesteps_edited_by_ui.push_back(mPlaybackSettings->current_timestep_index);
}

void ui::MDVisualiser::onShowEnergyLineGraphClicked()
{

    QWidget* energyGraphWindow = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(energyGraphWindow);
    layout->addWidget(mEnergyGraphWidget);
    energyGraphWindow->setLayout(layout);
    energyGraphWindow->show();
}

void ui::MDVisualiser::onShowKineticEnergyHistogramClicked()
{
    mPlaybackSettings->pause = true;
    KineticEnergyHistogramWidget* kineticEnergyHistogramWidget = new KineticEnergyHistogramWidget();
    std::vector<double> kineticEnergies;

    for (auto atom : current_timestep_data->atoms)
    {
        kineticEnergies.push_back(atom.kinetic_energy);
    }

    kineticEnergyHistogramWidget->setEnergies(kineticEnergies);

    QWidget* kineticEnergyHistogramWindow = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(kineticEnergyHistogramWindow);
    layout->addWidget(kineticEnergyHistogramWidget);
    kineticEnergyHistogramWindow->setLayout(layout);
    kineticEnergyHistogramWindow->show();
}