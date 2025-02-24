#pragma once
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSmartPointer.h>
#include <vtkCellPicker.h>
#include <vtkRenderer.h>


class AtomInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static AtomInteractorStyle* New();
    vtkTypeMacro(AtomInteractorStyle, vtkInteractorStyleTrackballCamera);

    // Provide a way to set/get the renderer if needed for picking
    void SetRenderer(vtkRenderer* renderer) { mRenderer = renderer; }
    vtkRenderer* GetRenderer() const { return mRenderer; }

    // Override the left button down event
    void OnLeftButtonDown() override
    {
        // // Get the (x, y) screen coordinates of the click
        // int* clickPos = this->GetInteractor()->GetEventPosition();

        // vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        // picker->SetTolerance(0.0005);

        // // Attempt the pick
        // bool picked = picker->Pick(clickPos[0],
        //                            clickPos[1],
        //                            0, // always zero for 2D screen click
        //                            mRenderer);

        // if (picked)
        // {
        //     // Example: get the actor or the pointId
        //     vtkIdType cellId = picker->GetCellId();
        //     double* pickedPos = picker->GetPickPosition();

        //     if (cellId >= 0)
        //     {
        //         std::cout << "Picked cell id = " << cellId
        //                   << ", world position = ("
        //                   << pickedPos[0] << ", "
        //                   << pickedPos[1] << ", "
        //                   << pickedPos[2] << ")\n";
                
        //         // If you're picking glyphs, you can also check the picked actor:
        //         //   if (picker->GetActor() == yourGlyphActor)
        //         //       ...
                
        //         // Or get a point id if you want to know exactly which atom in your data:
        //         vtkIdType pointId = picker->GetPointId();
        //         if (pointId >= 0)
        //         {
        //             std::cout << "Picked point id = " << pointId << std::endl;
        //             // We could couple point ids to the atom itself, and then use that to highlight the atom
        //         }
        //     }
        // }

        // Forward events to base class for camera rotation/zoom/pan
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

protected:
    AtomInteractorStyle() = default;
    ~AtomInteractorStyle() override = default;

private:
    vtkRenderer* mRenderer = nullptr;
};