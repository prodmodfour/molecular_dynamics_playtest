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
#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>




ui::AtomVTKWidget::AtomVTKWidget(QWidget* parent)
    : QVTKOpenGLNativeWidget(parent)
{
    mRenderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    this->setRenderWindow(mRenderWindow);
    mRenderWindow->SetSize(1280, 720);

    mRenderer = vtkSmartPointer<vtkRenderer>::New();
    mRenderWindow->AddRenderer(mRenderer);

    mPoints = vtkSmartPointer<vtkPoints>::New();
    mPolyData = vtkSmartPointer<vtkPolyData>::New();
    mPolyData->SetPoints(mPoints);

    mColors = vtkSmartPointer<vtkUnsignedCharArray>::New();
    mColors->SetNumberOfComponents(4);
    mColors->SetNumberOfTuples(0);
    mColors->SetName("Colors");
    mPolyData->GetPointData()->AddArray(mColors);

    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(1.28);

    mGlyphMapper = vtkSmartPointer<vtkGlyph3DMapper>::New();
    mGlyphMapper->SetInputData(mPolyData);
    mGlyphMapper->SetSourceConnection(sphereSource->GetOutputPort());
    mGlyphMapper->ScalarVisibilityOn();
    mGlyphMapper->SetScalarModeToUsePointFieldData();
    mGlyphMapper->SelectColorArray("Colors");
    mGlyphMapper->SetColorModeToDirectScalars();
    mGlyphMapper->SetUseLookupTableScalarRange(false);

    mGlyphActor = vtkSmartPointer<vtkActor>::New();
    mGlyphActor->SetMapper(mGlyphMapper);

    mRenderer->AddActor(mGlyphActor);

    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    mRenderer->SetBackground(colors->GetColor3d("Black").GetData());

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

    mRenderWindow->GetInteractor()->SetInteractorStyle(style);


    
}

void ui::AtomVTKWidget::updateAtoms(const std::vector<atoms::Atom>& atoms)
{


    mPoints->Reset();
    mPoints->SetNumberOfPoints(atoms.size());
    mColors->Reset();
    mColors->SetNumberOfTuples(atoms.size());

    for (size_t i = 0; i < atoms.size(); ++i)
    {
        mPoints->SetPoint(i, atoms[i].x, atoms[i].y, atoms[i].z);
        Color color = determine_colour_based_on_kinetic_energy(atoms[i].kinetic_energy, 0.01);
        mColors->SetTuple4(i, color.r, color.g, color.b, color.a);

    }

    mPoints->Modified();
    mPolyData->Modified();
    mColors->Modified();

    this->renderWindow()->Render();
}

void ui::AtomVTKWidget::render()
{
    this->renderWindow()->Render();
}


void ui::AtomVTKWidget::resetCameraToSystem()
{
    if (!mRenderer) return;

    mRenderer->ResetCamera();                   
    auto cam = mRenderer->GetActiveCamera();
    cam->Zoom(0.80);                           
    cam->Azimuth(45);                          
    cam->Elevation(25);                       
    mRenderer->ResetCameraClippingRange();
    this->renderWindow()->Render();
}