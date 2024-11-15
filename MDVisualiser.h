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

extern std::mutex timeline_mutex;

// Forward declarations
class MDVisualiser;
class TimerCallback;
class KeyPressCallback;
void get_atom_color(const Atom& atom, unsigned char color[4]);
void initialise_polydata(const std::vector<Atom>& all_atoms, vtkSmartPointer<vtkPolyData>& polyData);

// Timer callback class 
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

// KeyPressCallback class 
class KeyPressCallback : public vtkCommand {
public:
    static KeyPressCallback* New();
    void Execute(vtkObject* caller, unsigned long eventId, void* callData) override;
    void adjustPlaybackSpeed(int delta);
    MDVisualiser* visualiser;
};

class MDVisualiser : public QMainWindow {
    Q_OBJECT

public:
    MDVisualiser(SimulationData& sim_data, QWidget* parent = nullptr);
    void launch();



private:

    // Member variables
    SimulationData& simData;
    std::vector<Atom>& all_atoms;
    vtkSmartPointer<vtkPolyData> polyData;
    bool pauseAnimation;
    int playback_speed;
    int playback_direction;
    int frame_rate;

    // UI components
    vtkRenderWindowInteractor* interactor;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkPolyData> polyData;
    vtkSmartPointer<vtkTextActor> reading_actor;
    QVTKOpenGLNativeWidget* vtkWidget;

    friend class TimerCallback;
    friend class KeyPressCallback;
};



#endif
