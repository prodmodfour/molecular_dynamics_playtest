#include "../../../atoms/Atom.h"
#include "../../../atoms/AtomPairLibrary.h"
#include "../../../simulation/simulation_functions.h"
#include "../../../simulation/Config.h"
#include "../../../simulation/Timestep.h"
#include "../../../ui/MDVisualiser.h"
#include <vector>
#include <iostream>
#include <QApplication>

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





int main(int argc, char *argv[])
{   
    // QApplication app(argc, argv);
    // Test 1: Create a singe atom with no initial velocity. Watch it not move for 100 timesteps.



    // Create a single atom
    std::string atom_type = "Cu";
    double mass = 63.546;
    double radius = 1.28;
    atoms::Atom atom(atom_type, mass, radius);
    std::vector<atoms::Atom> atoms;
    atoms.push_back(atom);


    // Set up config for the simulation
    double r_cutoff = 0; // It's just one atom, so no cutoff is needed
    double timestep_size = 0.001;
    simulation::Config config(r_cutoff, timestep_size);


    // Set up Atom Pair Library
    atoms::AtomPairLibrary atom_pair_library;

    // Run the simulation
    std::vector<simulation::Timestep> simulation_data;
    int total_timesteps = 100;
    double time = 0;
    for (int i = 0; i < total_timesteps; i++) 
    {
        simulation::Timestep timestep = simulation::simulate_timestep(config, atoms, time, atom_pair_library);
        simulation_data.push_back(timestep);
    }

    // Print the simulation data
    for (simulation::Timestep timestep : simulation_data)
    {
        std::cout << "Timestep: " << timestep.time << " " << "Atom position: " << timestep.atoms[0].x << ", " << timestep.atoms[0].y << ", " << timestep.atoms[0].z << std::endl;
    }


    vtkSmartPointer<vtkGenericOpenGLRenderWindow> mRenderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    // Create a renderer and add it to the render window
    vtkSmartPointer<vtkRenderer> mRenderer = vtkSmartPointer<vtkRenderer>::New();
    mRenderWindow->AddRenderer(mRenderer);

    // Create points and poly data for atoms
    vtkSmartPointer<vtkPoints>  mPoints = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkPolyData>  mPolyData = vtkSmartPointer<vtkPolyData>::New();
    mPolyData->SetPoints(mPoints);

    // Configure the render window
    mRenderWindow->SetWindowName("Molecular Dynamics Playtest");
    mRenderWindow->SetSize(1280, 720);

    // Interactor (required for responding to mouse/keyboard, if needed)
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    renderWindowInteractor->SetRenderWindow(mRenderWindow);

    // Create a sphere source to use as a glyph
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(1.28);

    // Create glyph mapper
    vtkSmartPointer<vtkGlyph3DMapper> mGlyphMapper = vtkSmartPointer<vtkGlyph3DMapper>::New();
    mGlyphMapper->SetInputData(mPolyData);
    mGlyphMapper->SetSourceConnection(sphereSource->GetOutputPort());

    // Create an actor and set its mapper
    vtkSmartPointer<vtkActor> mGlyphActor = vtkSmartPointer<vtkActor>::New();
    mGlyphActor->SetMapper(mGlyphMapper);

    // Add the actor to the renderer
    mRenderer->AddActor(mGlyphActor);

    // Set a background color
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    mRenderer->SetBackground(colors->GetColor3d("Blue").GetData());

    mRenderWindow->Render();
    renderWindowInteractor->Subunttart();

    // Launch the visualiser

    // ui::MDVisualiser visualiser(simulation_data);
    // visualiser.show();
    // app.exec();



    return 0;
}


