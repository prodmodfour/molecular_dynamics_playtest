#include "AtomVTKWidget.h"
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
#include "../atoms/Atom.h"
#include <vtkPointData.h>
#include <vtkUnsignedCharArray.h>
#include "Color.h"
#include "visualisation_functions.h"



ui::AtomVTKWidget::AtomVTKWidget(QWidget* parent)
    : QVTKOpenGLNativeWidget(parent)
{
    // Create a VTK render window and associate it with this widget
    mRenderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    this->setRenderWindow(mRenderWindow);
    mRenderWindow->SetSize(1280, 720);

    // Create a renderer and add it to the render window
    mRenderer = vtkSmartPointer<vtkRenderer>::New();
    mRenderWindow->AddRenderer(mRenderer);


    // Create points and poly data for atoms
    mPoints = vtkSmartPointer<vtkPoints>::New();
    mPolyData = vtkSmartPointer<vtkPolyData>::New();
    mPolyData->SetPoints(mPoints);

    // Initialise color array
    mColors = vtkSmartPointer<vtkUnsignedCharArray>::New();
    mColors->SetNumberOfComponents(4);
    mColors->SetNumberOfTuples(0);
    mColors->SetName("Colors");
    mPolyData->GetPointData()->AddArray(mColors);



    // Create a sphere source to use as a glyph
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(1.28);

    // Create glyph mapper
    mGlyphMapper = vtkSmartPointer<vtkGlyph3DMapper>::New();
    mGlyphMapper->SetInputData(mPolyData);
    mGlyphMapper->SetSourceConnection(sphereSource->GetOutputPort());
    mGlyphMapper->ScalarVisibilityOn();
    mGlyphMapper->SetScalarModeToUsePointFieldData();
    mGlyphMapper->SelectColorArray("Colors");
    mGlyphMapper->SetColorModeToDirectScalars();
    mGlyphMapper->SetUseLookupTableScalarRange(false);

    // Create an actor and set its mapper
    mGlyphActor = vtkSmartPointer<vtkActor>::New();
    mGlyphActor->SetMapper(mGlyphMapper);

    // Add the actor to the renderer
    mRenderer->AddActor(mGlyphActor);

    // Set a background color
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    mRenderer->SetBackground(colors->GetColor3d("Black").GetData());

    // Set the camera
    mRenderer->ResetCamera();

    
}

void ui::AtomVTKWidget::updateAtoms(const std::vector<atoms::Atom>& atoms)
{
    // Clear current points
    mPoints->Reset();
    mPoints->SetNumberOfPoints(atoms.size());
    mColors->Reset();
    mColors->SetNumberOfTuples(atoms.size());

    // Populate vtkPoints with atom positions
    for (size_t i = 0; i < atoms.size(); ++i)
    {
        mPoints->SetPoint(i, atoms[i].x, atoms[i].y, atoms[i].z);
        Color color = determine_colour_based_on_kinetic_energy(atoms[i].kinetic_energy, 0.01);
        mColors->SetTuple4(i, color.r, color.g, color.b, color.a);

    }

    // Mark data as modified so VTK knows to update
    mPoints->Modified();
    mPolyData->Modified();
    mColors->Modified();

    // Request a render to update the view
    this->renderWindow()->Render();
}

void ui::AtomVTKWidget::render()
{
    this->renderWindow()->Render();
}


