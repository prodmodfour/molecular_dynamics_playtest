#pragma once

#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vector>
#include <string>

#include "../../atoms/Atom.h" 

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



 

    void updateAtoms();
    void renderImage();
    void resetCameraToSystem();
    void setAtomData(std::vector<atoms::Atom>* atoms);


    std::vector<std::string> get_structures_to_display();
    std::vector<atoms::Atom>* get_atoms();


private:
    void setupVTKPipeline();

    vtkSmartPointer<vtkGenericOpenGLRenderWindow> mRenderWindow;
    vtkSmartPointer<vtkRenderer> mRenderer;
    vtkSmartPointer<vtkPoints> mPoints;
    vtkSmartPointer<vtkPolyData> mPolyData;
    vtkSmartPointer<vtkUnsignedCharArray> mColors;
    vtkSmartPointer<vtkGlyph3DMapper> mGlyphMapper;
    vtkSmartPointer<vtkActor> mGlyphActor;
    std::vector<atoms::Atom>* atoms; 
    std::vector<std::string> structures_to_display;
    bool atom_data_is_set;
};

} // namespace ui


