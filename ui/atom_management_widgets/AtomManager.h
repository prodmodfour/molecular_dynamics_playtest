#pragma once

#include <QMainWindow>
#include "../MDVisualiser.h"
class QPushButton;

namespace ui
{

class AtomManager : public QMainWindow
{
    Q_OBJECT

public:
    AtomManager(QWidget* parent = nullptr);
    void onCloseButtonClicked();


private:
    QPushButton* mCloseButton;
    ui::MDVisualiser* parentMDVisualiser;

};

} // namespace ui