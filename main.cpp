#include <vector>
#include <vtkAnimationCue.h>
#include <vtkAnimationScene.h>
#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkLogger.h>
#include <vtkActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkVectorOperators.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkGlyph3DMapper.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkUnsignedCharArray.h>
#include "Atom.h"
#include "AtomGenerator.h"
#include "SimulationConfig.h"
#include "MDVisualiser.h"
#include <QApplication>
#include "Launcher.h"


#include "simulation.h"
#include "file_functions.h"
#include "SimulationData.h"
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <map>
#include <string>

int main(int argc, char* argv[) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.setWindowTitle("Simulation Configurator");
    mainWindow.show();

    return app.exec();
}

