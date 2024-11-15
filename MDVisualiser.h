#ifndef __MDVisualiser_h
#define __MDVisualiser_h

#include "SimulationConfig.h"
#include "SimulationData.h"
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkCommand.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <mutex>
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QTimer>
#include <QVTKOpenGLNativeWidget.h>

extern std::mutex timeline_mutex;

// Forward declarations
class MDVisualiser;
class TimerCallback;
class KeyPressCallback;
void get_atom_color(const Atom& atom, unsigned char color[3]);
void initialise_polydata(const std::vector<Atom>& all_atoms, vtkSmartPointer<vtkPolyData>& polyData);

// Timer callback class - keep the declaration but move implementation
class TimerCallback : public vtkCommand {
public:
    static TimerCallback* New();
    void Execute(vtkObject* caller, unsigned long eventId, void* callData) override;
    void updateSceneWithFrame(const Frame& frame);

    SimulationData* simData;
    vtkRenderWindow* renderWindow;
    vtkRenderer* renderer;
    vtkSmartPointer<vtkPolyData> polyData;
    vtkSmartPointer<vtkTextActor> reading_actor;
    MDVisualiser* visualiser;
};

// KeyPressCallback class - keep the declaration but move implementation
class KeyPressCallback : public vtkCommand {
public:
    static KeyPressCallback* New();
    void Execute(vtkObject* caller, unsigned long eventId, void* callData) override;
    void adjustPlaybackSpeed(int delta);
    MDVisualiser* visualiser;
};

class MDVisualiser {
public:
    MDVisualiser(SimulationData& sim_data);
    void launch(SimulationData& simData);

private:
    SimulationData& simData;
    std::vector<Atom>& all_atoms;
    vtkSmartPointer<vtkPolyData> polyData;
    bool pauseAnimation;
    int playback_speed;
    int playback_direction;
    int frame_rate;

    friend class TimerCallback;
    friend class KeyPressCallback;
};



#endif
