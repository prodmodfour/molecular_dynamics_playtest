#include "AtomVTKPreview.h"
#include "../../atoms/Atom.h" 
#include "../Color.h" 
#include "../visualisation_functions.h" 

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkUnsignedCharArray.h>
#include <vtkGlyph3DMapper.h>
#include <vtkActor.h>
#include <vtkSphereSource.h>
#include <vtkPointData.h>
#include <vtkNamedColors.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCamera.h>
#include <limits> 
#include <algorithm> 

namespace ui {

AtomVTKPreview::AtomVTKPreview(QWidget* parent)
    : QVTKOpenGLNativeWidget(parent) 
{
    setupVTKPipeline();
    atom_data_is_set = false;
    structures_to_display = std::vector<std::string>();
}

void AtomVTKPreview::setupVTKPipeline() 
{
    mRenderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    this->setRenderWindow(mRenderWindow);

    mRenderer = vtkSmartPointer<vtkRenderer>::New();
    mRenderWindow->AddRenderer(mRenderer);

    mPoints = vtkSmartPointer<vtkPoints>::New();
    mPolyData = vtkSmartPointer<vtkPolyData>::New();
    mPolyData->SetPoints(mPoints);

    mColors = vtkSmartPointer<vtkUnsignedCharArray>::New();
    mColors->SetNumberOfComponents(4); // RGBA
    mColors->SetName("Colors");
    mPolyData->GetPointData()->AddArray(mColors);

    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();

    sphereSource->SetRadius(1.28); 


    mGlyphMapper = vtkSmartPointer<vtkGlyph3DMapper>::New();
    mGlyphMapper->SetInputData(mPolyData);
    mGlyphMapper->SetSourceConnection(sphereSource->GetOutputPort());
    mGlyphMapper->SetScaleModeToScaleByMagnitude(); 
    mGlyphMapper->SetScaleFactor(1.0); 

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

    // Create and set the interactor style
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    if (mRenderWindow->GetInteractor()) {
         mRenderWindow->GetInteractor()->SetInteractorStyle(style);
    }
   
    // Reset camera for initial view
    mRenderer->ResetCamera();
    mRenderer->GetActiveCamera()->Zoom(1.5); 
}



void AtomVTKPreview::setAtomData(std::vector<atoms::Atom>* atoms) 
{
    this->atoms = atoms;
    atom_data_is_set = true;
}

void AtomVTKPreview::updateAtoms()
{
    if (!atoms || atoms->empty()) {
        return; 
    }

    mPoints->Reset();
    mColors->Reset();
    mColors->SetNumberOfTuples(atoms->size()); 



    vtkIdType pointId = 0;
    for (auto& atom : *atoms) {
        if (std::find(structures_to_display.begin(), structures_to_display.end(), atom.parent_structure) == structures_to_display.end()) {
            continue;
        }

        mPoints->InsertNextPoint(atom.x, atom.y, atom.z);

        Color atomColor = determine_colour_based_on_kinetic_energy(atom.kinetic_energy, 0.01); 
        unsigned char color[4] = {atomColor.r, atomColor.g, atomColor.b, atomColor.a};
        mColors->SetTypedTuple(pointId++, color);
    }

    mPoints->Modified();
    mColors->Modified();
    mPolyData->Modified();

    mGlyphMapper->SetInputData(mPolyData); 

    mRenderer->ResetCameraClippingRange();
    mRenderer->ResetCamera();
    renderImage();
}

void AtomVTKPreview::renderImage() 
{
    if (mRenderWindow) {
        mRenderWindow->Render();
    }
}

void AtomVTKPreview::resetCameraToSystem()
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

} // namespace ui
