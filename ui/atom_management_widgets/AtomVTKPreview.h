#pragma once

#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vector>
#include <string>

#include "../../atoms/Atom.h" 

// Forward declarations for VTK classes
class vtkGenericOpenGLRenderWindow;
class vtkRenderer;
class vtkPoints;
class vtkPolyData;
class vtkUnsignedCharArray;
class vtkGlyph3DMapper;
class vtkActor;


namespace ui {

class AtomVTKPreview : public QVTKOpenGLNativeWidget { 
    Q_OBJECT

public:
    explicit AtomVTKPreview(QWidget* parent = nullptr);
    virtual ~AtomVTKPreview() = default;

    std::vector<atoms::Atom>* atoms; 
    std::vector<std::string> structures_to_display;
    bool atom_data_is_set;

    void setAtomData(std::vector<atoms::Atom>* atoms); 

    void updateAtoms();
    void renderImage();
    void resetCameraToSystem();


private:
    void setupVTKPipeline();

    vtkSmartPointer<vtkGenericOpenGLRenderWindow> mRenderWindow;
    vtkSmartPointer<vtkRenderer> mRenderer;
    vtkSmartPointer<vtkPoints> mPoints;
    vtkSmartPointer<vtkPolyData> mPolyData;
    vtkSmartPointer<vtkUnsignedCharArray> mColors;
    vtkSmartPointer<vtkGlyph3DMapper> mGlyphMapper;
    vtkSmartPointer<vtkActor> mGlyphActor;
};

} // namespace ui


