#include "AtomStructureVTKPreview.h"
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


namespace ui {

AtomStructureVTKPreview::AtomStructureVTKPreview(QWidget* parent)
    : QVTKOpenGLNativeWidget(parent) 
{
    setupVTKPipeline();
    atom_data_is_set = false;
}

void AtomStructureVTKPreview::setupVTKPipeline() 
{
    // Create a VTK render window and associate it with this widget
    mRenderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    this->setRenderWindow(mRenderWindow);

    // Create a renderer and add it to the render window
    mRenderer = vtkSmartPointer<vtkRenderer>::New();
    mRenderWindow->AddRenderer(mRenderer);

    // Create points and poly data for atoms
    mPoints = vtkSmartPointer<vtkPoints>::New();
    mPolyData = vtkSmartPointer<vtkPolyData>::New();
    mPolyData->SetPoints(mPoints);

    // Initialise color array
    mColors = vtkSmartPointer<vtkUnsignedCharArray>::New();
    mColors->SetNumberOfComponents(4); // RGBA
    mColors->SetName("Colors");
    mPolyData->GetPointData()->AddArray(mColors);

    // Create a sphere source to use as a glyph
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    // Use a default radius, can be adjusted later if needed
    // We use the radius of copper as our default
    sphereSource->SetRadius(1.28); 

    // Create glyph mapper
    mGlyphMapper = vtkSmartPointer<vtkGlyph3DMapper>::New();
    mGlyphMapper->SetInputData(mPolyData);
    mGlyphMapper->SetSourceConnection(sphereSource->GetOutputPort());
    mGlyphMapper->SetScaleModeToScaleByMagnitude(); // Or another scale mode if desired
    mGlyphMapper->SetScaleFactor(1.0); // Adjust scale factor as needed

    // Configure color handling
    mGlyphMapper->ScalarVisibilityOn();
    mGlyphMapper->SetScalarModeToUsePointFieldData();
    mGlyphMapper->SelectColorArray("Colors");
    mGlyphMapper->SetColorModeToDirectScalars(); // Use the RGBA values directly
    mGlyphMapper->SetUseLookupTableScalarRange(false);

    // Create an actor and set its mapper
    mGlyphActor = vtkSmartPointer<vtkActor>::New();
    mGlyphActor->SetMapper(mGlyphMapper);

    // Add the actor to the renderer
    mRenderer->AddActor(mGlyphActor);

    // Set a background color
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


void AtomStructureVTKPreview::setAtomData(std::vector<Atom>* atoms) 
{
    this->atoms = atoms
    atom_data_is_set = true;
}

void AtomStructureVTKPreview::updateAtoms()
{
    mPoints->Reset();
    mColors->Reset();
    mColors->SetNumberOfTuples(atoms->size()); 

    vtkIdType pointId = 0;
    for (auto& atom : atoms) {
        mPoints->InsertNextPoint(atom.x, atom.y, atom.z);

        // Get color based on atom type/element
        Color atomColor = determine_colour_based_on_kinetic_energy; 
        unsigned char color[4] = {atomColor.r, atomColor.g, atomColor.b, atomColor.a};
        mColors->SetTypedTuple(pointId++, color);
    }

    // Notify VTK that the data has changed
    mPoints->Modified();
    mColors->Modified();
    mPolyData->Modified();


    // Update the glyph mapper input
    mGlyphMapper->SetInputData(mPolyData); // Re-set input data if needed

    // Adjust camera to fit the new data
    mRenderer->ResetCameraClippingRange();
    mRenderer->ResetCamera();
}

void AtomStructureVTKPreview::renderImage() 
{
    if (mRenderWindow) {
        mRenderWindow->Render();
    }
}

} // namespace ui
