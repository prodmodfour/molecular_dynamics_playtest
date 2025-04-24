#pragma once

#include <QMainWindow>

class QPushButton;

// Forward declaration of MDVisualiser to break the circular dependency
namespace ui {
    class MDVisualiser;
}

namespace ui
{

class AtomManager : public QMainWindow
{
    Q_OBJECT

public:
    AtomManager(QWidget* parent = nullptr);
    void onCloseButtonClicked();
    void setParentMDVisualiser(MDVisualiser* visualiser);


private:
    QPushButton* mCloseButton;
    MDVisualiser* parentMDVisualiser; // Use the forward-declared class

};

} // namespace ui