#pragma once

#include <QVTKOpenGLNativeWidget.h>
#include <QObject>
#include <vector>

#include "../simulation/Atom.h"

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkPolyData.h>
#include <vtkGlyph3DMapper.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkNamedColors.h>
#include <vtkRenderWindow.h>
#include <vtkProperty.h>
#include <vtkPoints.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>





class AtomVTKWidget : public QVTKOpenGLNativeWidget
{
    Q_OBJECT
public:
    explicit AtomVTKWidget(QWidget* parent = nullptr);

    void updateAtoms(const std::vector<Atom>& atoms);

private:
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> mRenderWindow;
    vtkSmartPointer<vtkRenderer>                  mRenderer;
    vtkSmartPointer<vtkPoints>                    mPoints;
    vtkSmartPointer<vtkPolyData>                  mPolyData;
    vtkSmartPointer<vtkGlyph3DMapper>             mGlyphMapper;
    vtkSmartPointer<vtkActor>                     mGlyphActor;
};
