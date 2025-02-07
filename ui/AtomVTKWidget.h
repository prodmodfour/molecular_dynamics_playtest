#pragma once

#include <QVTKOpenGLNativeWidget.h>
#include <QObject>
#include <vector>

#include "../simulation/Atom.h"




namespace vtk
{
    // Forward declarations to avoid including all VTK headers here
    class GenericOpenGLRenderWindow;
    class Renderer;
    class Points;
    class PolyData;
    class Glyph3DMapper;
    class Actor;
}

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
