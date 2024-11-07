// MDMainWindow.h
#ifndef MD_MAIN_WINDOW_H
#define MD_MAIN_WINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QTimer>
#include <QDockWidget>
#include <QGroupBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QTabWidget>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include "SimulationHistory.h"
#include "Settings.h"

class MDMainWindow : public QMainWindow {
    Q_OBJECT

public:
    MDMainWindow(SimulationHistory* simData, Settings* settings, QWidget* parent = nullptr)
        : QMainWindow(parent), simulation(simData), simSettings(settings) {
        setupUi();
        connectSignals();
        updateTimer = new QTimer(this);
        connect(updateTimer, &QTimer::timeout, this, &MDMainWindow::updateSimulationDisplay);
        updateTimer->start(33); // ~30 FPS
    }

    vtkRenderer* getRenderer() { return renderer; }
    vtkRenderWindow* getRenderWindow() { return renderWindow; }

private slots:
    void playPauseSimulation() {
        pauseAnimation = !pauseAnimation;
        playPauseButton->setText(pauseAnimation ? "Play" : "Pause");
    }

    void reversePlayback() {
        playback_direction *= -1;
        directionButton->setText(playback_direction > 0 ? "Forward" : "Reverse");
    }

    void updateSimulationDisplay() {
        if (!pauseAnimation) {
            for (int i = 0; i < playbackSpeedBox->value(); i++) {
                if (playback_direction > 0) {
                    if (!simulation->move_forward()) break;
                } else {
                    if (!simulation->move_backward()) break;
                }
            }
            updateEnergyDisplay();
            renderWindow->Render();
        }
    }

    void updateEnergyDisplay() {
        Snapshot frame = simulation->get_current_frame();
        QString stats = QString("Time: %1 ps | Total Energy: %2 eV | KE: %3 eV | PE: %4 eV")
            .arg(frame.time, 0, 'f', 3)
            .arg(frame.te, 0, 'f', 2)
            .arg(frame.ke, 0, 'f', 2)
            .arg(frame.pe, 0, 'f', 2);
        energyLabel->setText(stats);
    }

    void impactEnergyChanged(double value) {
        Settings& settings = simulation->get_branch_settings(simulation->get_active_branch_id());
        settings.set_energy_applied_to_impact_atom(value);
    }

    void bombardmentIntervalChanged(double value) {
        Settings& settings = simulation->get_branch_settings(simulation->get_active_branch_id());
        settings.set_bombardment_interval(value);
    }

    void toggleBombardment(bool enabled) {
        Settings& settings = simulation->get_branch_settings(simulation->get_active_branch_id());
        settings.set_bombardment_on(enabled);
        bombardmentGroup->setEnabled(enabled);
    }

    void createNewBranch() {
        Settings branch_settings = simulation->get_current_settings();
        branch_settings.set_energy_applied_to_impact_atom(
            impactEnergySpinBox->value()
        );
        std::string new_branch_id = "branch_" + std::to_string(time(nullptr));
        simulation->create_branch(new_branch_id, branch_settings);
        updateBranchList();
    }

    void switchBranch(const QString& branchId) {
        simulation->switch_branch(branchId.toStdString());
        updateSettingsDisplay();
    }

private:
    void setupUi() {
        // Create central VTK widget
        QVTKOpenGLNativeWidget* vtkWidget = new QVTKOpenGLNativeWidget();
        renderWindow = vtkGenericOpenGLRenderWindow::New();
        vtkWidget->setRenderWindow(renderWindow);
        renderer = vtkRenderer::New();
        renderWindow->AddRenderer(renderer);
        setCentralWidget(vtkWidget);

        // Create control dock widget
        QDockWidget* controlDock = new QDockWidget("Simulation Controls", this);
        QWidget* controlWidget = new QWidget(controlDock);
        QVBoxLayout* controlLayout = new QVBoxLayout(controlWidget);

        // Playback controls
        QGroupBox* playbackGroup = new QGroupBox("Playback", controlWidget);
        QHBoxLayout* playbackLayout = new QHBoxLayout(playbackGroup);
        playPauseButton = new QPushButton("Pause", playbackGroup);
        directionButton = new QPushButton("Forward", playbackGroup);
        playbackSpeedBox = new QSpinBox(playbackGroup);
        playbackSpeedBox->setRange(1, 10);
        playbackSpeedBox->setValue(1);
        playbackSpeedBox->setPrefix("Speed: x");
        
        playbackLayout->addWidget(playPauseButton);
        playbackLayout->addWidget(directionButton);
        playbackLayout->addWidget(playbackSpeedBox);
        
        // Impact controls
        QGroupBox* impactGroup = new QGroupBox("Impact Settings", controlWidget);
        QFormLayout* impactLayout = new QFormLayout(impactGroup);
        impactEnergySpinBox = new QDoubleSpinBox(impactGroup);
        impactEnergySpinBox->setRange(0, 1000);
        impactEnergySpinBox->setValue(simSettings->get_energy_applied_to_impact_atom());
        impactEnergySpinBox->setSuffix(" eV");
        
        // Bombardment controls
        QGroupBox* bombardmentGroup = new QGroupBox("Bombardment", controlWidget);
        QFormLayout* bombardmentLayout = new QFormLayout(bombardmentGroup);
        bombardmentCheck = new QCheckBox("Enable Bombardment", bombardmentGroup);
        bombardmentIntervalBox = new QDoubleSpinBox(bombardmentGroup);
        bombardmentIntervalBox->setRange(0.01, 1.0);
        bombardmentIntervalBox->setValue(simSettings->get_bombardment_interval());
        bombardmentIntervalBox->setSuffix(" ps");
        
        bombardmentModeBox = new QComboBox(bombardmentGroup);
        bombardmentModeBox->addItems({"consistent", "spread", "3d_spread"});
        
        bombardmentLayout->addRow(bombardmentCheck);
        bombardmentLayout->addRow("Interval:", bombardmentIntervalBox);
        bombardmentLayout->addRow("Mode:", bombardmentModeBox);
        
        // Branch controls
        QGroupBox* branchGroup = new QGroupBox("Simulation Branches", controlWidget);
        QVBoxLayout* branchLayout = new QVBoxLayout(branchGroup);
        branchComboBox = new QComboBox(branchGroup);
        QPushButton* newBranchButton = new QPushButton("Create New Branch", branchGroup);
        
        branchLayout->addWidget(branchComboBox);
        branchLayout->addWidget(newBranchButton);
        
        // Energy display
        energyLabel = new QLabel(controlWidget);
        energyLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        
        // Add all groups to control layout
        controlLayout->addWidget(playbackGroup);
        controlLayout->addWidget(impactGroup);
        controlLayout->addWidget(bombardmentGroup);
        controlLayout->addWidget(branchGroup);
        controlLayout->addWidget(energyLabel);
        controlLayout->addStretch();
        
        controlDock->setWidget(controlWidget);
        addDockWidget(Qt::RightDockWidgetArea, controlDock);

        // Initialize branch list
        updateBranchList();
    }

    void connectSignals() {
        connect(playPauseButton, &QPushButton::clicked, this, &MDMainWindow::playPauseSimulation);
        connect(directionButton, &QPushButton::clicked, this, &MDMainWindow::reversePlayback);
        connect(impactEnergySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                this, &MDMainWindow::impactEnergyChanged);
        connect(bombardmentCheck, &QCheckBox::toggled, this, &MDMainWindow::toggleBombardment);
        connect(bombardmentIntervalBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                this, &MDMainWindow::bombardmentIntervalChanged);
        connect(branchComboBox, &QComboBox::currentTextChanged, this, &MDMainWindow::switchBranch);
    }

    void updateBranchList() {
        branchComboBox->clear();
        for (const auto& branch : simulation->get_available_branches()) {
            branchComboBox->addItem(QString::fromStdString(branch));
        }
    }

    void updateSettingsDisplay() {
        Settings& settings = simulation->get_current_settings();
        impactEnergySpinBox->setValue(settings.get_energy_applied_to_impact_atom());
        bombardmentCheck->setChecked(settings.get_bombardment_on());
        bombardmentIntervalBox->setValue(settings.get_bombardment_interval());
        int index = bombardmentModeBox->findText(QString::fromStdString(settings.get_bombardment_mode()));
        if (index >= 0) bombardmentModeBox->setCurrentIndex(index);
    }

    SimulationHistory* simulation;
    Settings* simSettings;
    vtkRenderer* renderer;
    vtkRenderWindow* renderWindow;
    QTimer* updateTimer;
    
    // GUI elements
    QPushButton* playPauseButton;
    QPushButton* directionButton;
    QSpinBox* playbackSpeedBox;
    QDoubleSpinBox* impactEnergySpinBox;
    QCheckBox* bombardmentCheck;
    QDoubleSpinBox* bombardmentIntervalBox;
    QComboBox* bombardmentModeBox;
    QComboBox* branchComboBox;
    QGroupBox* bombardmentGroup;
    QLabel* energyLabel;
};

#endif