#pragma once

#include <QVTKOpenGLNativeWidget.h> // or QVTKOpenGLWidget, depending on your setup
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

class AtomVTKWidget : public QVTKOpenGLNativeWidget
{
    Q_OBJECT
public:
    explicit AtomVTKWidget(QWidget* parent = nullptr)
        : QVTKOpenGLNativeWidget(parent)
    {
        // Create a VTK render window and renderer
        mRenderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
        this->setRenderWindow(mRenderWindow);

        mRenderer = vtkSmartPointer<vtkRenderer>::New();
        mRenderWindow->AddRenderer(mRenderer);

        // Setup pipeline objects
        mPoints = vtkSmartPointer<vtkPoints>::New();
        mPolyData = vtkSmartPointer<vtkPolyData>::New();
        mPolyData->SetPoints(mPoints);

        // Create a sphere source to use as glyph
        vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
        sphereSource->SetRadius(1.28); // adjust radius as needed

        // Create glyph mapper
        mGlyphMapper = vtkSmartPointer<vtkGlyph3DMapper>::New();
        mGlyphMapper->SetInputData(mPolyData);
        mGlyphMapper->SetSourceConnection(sphereSource->GetOutputPort());

        // Create an actor
        mGlyphActor = vtkSmartPointer<vtkActor>::New();
        mGlyphActor->SetMapper(mGlyphMapper);

        // Add actor to the renderer
        mRenderer->AddActor(mGlyphActor);

        // Set background color
        vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
        mRenderer->SetBackground(colors->GetColor3d("SlateGray").GetData());
    }

    void updateAtoms(const std::vector<Atom>& atoms)
    {
        // Clear current points
        mPoints->Reset();
        mPoints->SetNumberOfPoints(atoms.size());

        // Add each atom position into vtkPoints
        for (size_t i = 0; i < atoms.size(); ++i)
        {
            mPoints->SetPoint(i, atoms[i].x, atoms[i].y, atoms[i].z);
        }

        // Let VTK know we've modified the data
        mPoints->Modified();
        mPolyData->Modified();

        // Request a render
        this->renderWindow()->Render();
    }

private:
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> mRenderWindow;
    vtkSmartPointer<vtkRenderer>                  mRenderer;
    vtkSmartPointer<vtkPoints>                    mPoints;
    vtkSmartPointer<vtkPolyData>                  mPolyData;
    vtkSmartPointer<vtkGlyph3DMapper>            mGlyphMapper;
    vtkSmartPointer<vtkActor>                     mGlyphActor;
};
