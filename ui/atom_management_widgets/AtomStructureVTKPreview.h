#ifndef ATOMSTRUCTUREVTKPREVIEW_H
#define ATOMSTRUCTUREVTKPREVIEW_H

#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vector>

// Forward declarations for VTK classes
class vtkGenericOpenGLRenderWindow;
class vtkRenderer;
class vtkPoints;
class vtkPolyData;
class vtkUnsignedCharArray;
class vtkGlyph3DMapper;
class vtkActor;
class Atom; // Forward declaration for Atom class

namespace ui {
namespace atom_management {

class AtomStructureVTKPreview : public QVTKOpenGLNativeWidget {
    Q_OBJECT

public:
    explicit AtomStructureVTKPreview(QWidget* parent = nullptr);
    virtual ~AtomStructureVTKPreview() = default;

    /**
     * @brief Sets the atom data to be displayed in the preview.
     *
     * This function updates the VTK pipeline with the provided atom positions
     * and types, and then triggers a render.
     *
     * @param atoms A vector of Atom objects to display.
     */
    void setAtomData(const std::vector<Atom>& atoms);

    /**
     * @brief Renders the current scene.
     *
     * Forces the VTK render window to render the scene with the current atom data.
     */
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

} // namespace atom_management
} // namespace ui

#endif // ATOMSTRUCTUREVTKPREVIEW_H
