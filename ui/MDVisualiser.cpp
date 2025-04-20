#include "MDVisualiser.h"
#include "../simulation/Timestep.h"
#include <stdexcept>
#include "data_loaders/BasicDataLoader.h"
#include "atom_management_widgets/AtomStructureInserter.h" 

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


    // Create a central widget and main layout
    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // Create our Atom VTK Widget
    mVTKWidget = new AtomVTKWidget(central);
    mVTKWidget->setFixedSize(1280, 720);
    mainLayout->addWidget(mVTKWidget);


    // --- Playback Controls ---
    QHBoxLayout* controlsLayout = new QHBoxLayout;

    // Speed controls
    mSpeedDownButton = new QPushButton("-", central);
    controlsLayout->addWidget(mSpeedDownButton);

    mSpeedLineEdit = new QLineEdit(central);
    mSpeedLineEdit->setText(QString::number(mPlaybackSettings->speed));
    mSpeedLineEdit->setFixedWidth(50); // just to limit the size a bit
    controlsLayout->addWidget(mSpeedLineEdit);

    mSpeedUpButton = new QPushButton("+", central);
    controlsLayout->addWidget(mSpeedUpButton);

    // Start/Pause button
    mStartPauseButton = new QPushButton("Start/Pause", central);
    controlsLayout->addWidget(mStartPauseButton);

    // Reverse button
    mReverseButton = new QPushButton("Reverse", central);
    controlsLayout->addWidget(mReverseButton);

    // Restart button
    mRestartButton = new QPushButton("Restart", central);
    controlsLayout->addWidget(mRestartButton);

    // Add Atoms button
    mAddAtomsButton = new QPushButton("Add Atoms", central); // Instantiate the button
    controlsLayout->addWidget(mAddAtomsButton); // Add to layout

    connect(mRestartButton, &QPushButton::clicked,
            this, &ui::MDVisualiser::onRestartClicked);

    mainLayout->addLayout(controlsLayout);
    setCentralWidget(central);

    // Connect speed controls
    connect(mSpeedDownButton, &QPushButton::clicked,
            this, &MDVisualiser::onSpeedDownClicked);
    connect(mSpeedUpButton, &QPushButton::clicked,
            this, &MDVisualiser::onSpeedUpClicked);
    connect(mSpeedLineEdit, &QLineEdit::editingFinished,
            this, &MDVisualiser::onSpeedLineEditChanged);

    // Connect start/pause button
    connect(mStartPauseButton, &QPushButton::clicked,
            this, &ui::MDVisualiser::onStartPauseClicked);

    // Connect reverse button
    connect(mReverseButton, &QPushButton::clicked,
            this, &ui::MDVisualiser::onReverseClicked);

    // Connect Add Atoms button
    connect(mAddAtomsButton, &QPushButton::clicked, // Connect the signal
            this, &ui::MDVisualiser::onAddAtomsClicked); // To the slot

    // Setup timer to update animation
    mTimer = new QTimer(this);
    connect(mTimer, &QTimer::timeout,
            this, &ui::MDVisualiser::onTimerTimeout);
    mTimer->start(42); // update every 42 ms (approx. 24 FPS)



}

void ui::MDVisualiser::onTimerTimeout()
{
    if (mPlaybackSettings->pause == false)
    {
        mPlaybackSettings->next_timestep();
    }

    if (mDataLoader->load())
    {
        // // // Render the new timestep
        mVTKWidget->updateAtoms(current_timestep_data->atoms);
    }

}

void ui::MDVisualiser::onSpeedDownClicked()
{
    bool ok;
    int currentSpeed = mSpeedLineEdit->text().toInt(&ok);
    if (!ok) return; // Invalid input, do nothing
    if (currentSpeed > 1)
    {
        currentSpeed--;
        mSpeedLineEdit->setText(QString::number(currentSpeed));
        mPlaybackSettings->change_speed(currentSpeed);
    }
}

void ui::MDVisualiser::onSpeedUpClicked()
{
    bool ok;
    int currentSpeed = mSpeedLineEdit->text().toInt(&ok);
    if (!ok) return; // Invalid input, do nothing
    
    currentSpeed++;
    mSpeedLineEdit->setText(QString::number(currentSpeed));
    mPlaybackSettings->change_speed(currentSpeed);
}

void ui::MDVisualiser::onSpeedLineEditChanged()
{
    bool ok;
    int newSpeed = mSpeedLineEdit->text().toInt(&ok);
    if (!ok) return; // Invalid input, revert or ignore

    mPlaybackSettings->change_speed(newSpeed);
}

void ui::MDVisualiser::onStartPauseClicked()
{
    mPlaybackSettings->toggle_pause();
}

void ui::MDVisualiser::onReverseClicked()
{
    mPlaybackSettings->change_direction();
}

void ui::MDVisualiser::onRestartClicked()
{
    // Signal the application event loop to exit with code 1:
    qApp->exit(1);
}

// Implementation for the onAddAtomsClicked slot
void ui::MDVisualiser::onAddAtomsClicked()
{
    mPlaybackSettings->pause = true;
    AtomStructureInserter inserterDialog(this);
    if (inserterDialog.exec() == QDialog::Accepted) { 

        AtomStructureInserter::AtomStructureParameters params = inserterDialog.getParameters();

        std::cout << "Atom Structure Inserter accepted." << std::endl;

        // print paramaters
        std::cout << "Structure Type: " << params.structureType.toStdString() << std::endl;
        std::cout << "Atom Type: " << params.atomType.toStdString() << std::endl;
        std::cout << "Atom Radius: " << params.atomRadius << std::endl;
        std::cout << "Center: (" << params.center.x << ", " << params.center.y << ", " << params.center.z << ")" << std::endl;
        std::cout << "Cubes in X: " << params.fccParams.cubes.x << std::endl;
        std::cout << "Cubes in Y: " << params.fccParams.cubes.y << std::endl;
        std::cout << "Cubes in Z: " << params.fccParams.cubes.z << std::endl;
        std::cout << "Atom Spacing: " << params.fccParams.spacing << std::endl;
        std::cout << "Apply Kinetic Energy: " << (params.applyKineticEnergy ? "Yes" : "No") << std::endl;
        std::cout << "Kinetic Energy: " << params.kineticEnergyParams.kineticEnergy << std::endl;
        std::cout << "Target Coordinates: (" << params.kineticEnergyParams.targetCoordinates.x << ", " << params.kineticEnergyParams.targetCoordinates.y << ", " << params.kineticEnergyParams.targetCoordinates.z << ")" << std::endl;
        std::cout << "Offset: (" << params.kineticEnergyParams.offset.x << ", " << params.kineticEnergyParams.offset.y << ", " << params.kineticEnergyParams.offset.z << ")" << std::endl;
        
    }
    else {
        std::cout << "Atom Structure Inserter canceled." << std::endl;
    }
    mPlaybackSettings->pause = false;
}


void ui::MDVisualiser::setDataLoader(ui::BasicDataLoader* data_loader)
{
    // Setting the data loader sets the data loader's playback settings and visualiser
    // This is a similar way of linking objects to the way that VTK does it.
    mDataLoader = data_loader;
    mDataLoader->setPlaybackSettings(mPlaybackSettings);
    mDataLoader->setVisualiser(this);
}

void ui::MDVisualiser::setPlaybackSettings(ui::PlaybackSettings* playback_settings)
{
    mPlaybackSettings = playback_settings;
}

