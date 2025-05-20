#pragma once

#include <QDialog>


class SharedData;

namespace ui
{

class SimulationSettingsDialogue : public QDialog
{
    Q_OBJECT

public:
    SimulationSettingsDialogue(QWidget* parent = nullptr, SharedData* shared_data = nullptr);

    void setSharedData(SharedData* shared_data);
    void applyChanges();
    
public slots:
    void onApplyClicked();
    void onCancelClicked();
private:
    SharedData* shared_data;


};
} // namespace ui
