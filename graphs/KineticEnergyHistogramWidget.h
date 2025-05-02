#pragma once

#include <QWidget>
#include <QVector>
#include <vector>


class KineticEnergyHistogramWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KineticEnergyHistogramWidget(QWidget* parent = nullptr);


    void setEnergies(const std::vector<double>& energies);


    void setBinCount(int bins);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void recomputeBins();

    // -----------------------------------------------------------------------
    std::vector<double> m_data;
    int                 m_binCount;
    QVector<int>        m_binFrequencies;
    double              m_minEnergy;
    double              m_maxEnergy;
};

