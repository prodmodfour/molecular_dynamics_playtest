#pragma once

#include <QDialog>

class QLineEdit;
class QComboBox;
class QSpinBox;
class QDoubleSpinBox;

namespace ui
{
    struct StructureParams
    {
        QString structureName;
        QString structureType;
        int     cubesX  = 4;
        int     cubesY  = 4;
        int     cubesZ  = 4;
        QString atomType;     
        double  atomSpacing = 3.61;   
        double  offsetX = 0.0;
        double  offsetY = 0.0;
        double  offsetZ = 0.0;
    };

    class NewStructureDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewStructureDialog(QWidget *parent = nullptr);
    void onAccept();
    StructureParams get_params();




private:
    QLineEdit      *m_nameEdit;
    
    QComboBox      *m_structureCB;
    QSpinBox       *m_cubesXSB;
    QSpinBox       *m_cubesYSB;
    QSpinBox       *m_cubesZSB;
    QComboBox      *m_atomTypeCB;
    QDoubleSpinBox *m_spacingDSB;
    QDoubleSpinBox *m_offsetXDSB;
    QDoubleSpinBox *m_offsetYDSB;
    QDoubleSpinBox *m_offsetZDSB;
    QString structureType() const;
    int     cubesX()        const;
    int     cubesY()        const;
    int     cubesZ()        const;
    QString atomType()      const;
    double  atomSpacing()   const;
    StructureParams m_params;


};

} // namespace ui