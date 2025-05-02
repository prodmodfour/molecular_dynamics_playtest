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
        QString structureType;   // "fcc" | "bcc"
        int     cubesX  = 4;
        int     cubesY  = 4;
        int     cubesZ  = 4;
        QString atomType;        // "Cu"
        double  atomSpacing = 3.61;   // Å
    };

    class NewStructureDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewStructureDialog(QWidget *parent = nullptr);
    void onAccept();


    QString structureType() const;
    int     cubesX()        const;
    int     cubesY()        const;
    int     cubesZ()        const;
    QString atomType()      const;
    double  atomSpacing()   const;
    StructureParams m_params;

private:
    QLineEdit      *m_nameEdit;
    
    QComboBox      *m_structureCB;
    QSpinBox       *m_cubesXSB;
    QSpinBox       *m_cubesYSB;
    QSpinBox       *m_cubesZSB;
    QComboBox      *m_atomTypeCB;
    QDoubleSpinBox *m_spacingDSB;


};

} // namespace ui