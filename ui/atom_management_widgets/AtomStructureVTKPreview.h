#ifndef ATOMSTRUCTUREVTKPREVIEW_H
#define ATOMSTRUCTUREVTKPREVIEW_H

#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vector>

#include "../../atoms/Atom.h" // Include the full definition of Atom

// Forward declarations for VTK classes
class vtkGenericOpenGLRenderWindow;
class vtkRenderer;
class vtkPoints;
class vtkPolyData;
class vtkUnsignedCharArray;
class vtkGlyph3DMapper;
class vtkActor;
// class Atom; // Remove forward declaration, we included the header

namespace ui {

class AtomStructureVTKPreview : public QVTKOpenGLNativeWidget { // I'd ordinarily add widget to the end of this name but it's already long
    Q_OBJECT

public:
    explicit AtomStructureVTKPreview(QWidget* parent = nullptr);
    virtual ~AtomStructureVTKPreview() = default;

    std::vector<atoms::Atom>* atoms; // Use namespace for Atom
    bool atom_data_is_set;

    void setAtomData(std::vector<atoms::Atom>* atoms); // Use namespace for Atom

    void updateAtoms();
    void renderImage();
    


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

#endif // ATOMSTRUCTUREVTKPREVIEW_H
