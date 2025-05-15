#pragma once

#include <QWidget>
#include <QtCharts/QChart>         
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QTimer>
#include <QVBoxLayout>




namespace graphs
{
    class EnergyGraphWidget : public QWidget
    {
        Q_OBJECT
public:
    explicit EnergyGraphWidget(QWidget *parent = nullptr);

    void addDataPoint(qreal time, qreal kinetic, qreal potential);

private slots:


private:
    QLineSeries *m_kineticSeries;
    QLineSeries *m_potentialSeries;
    QLineSeries *m_totalSeries;
    QChart       *m_chart;
    QValueAxis   *m_axisX;
    QValueAxis   *m_axisY;
    QTimer       *m_timer;
    qreal                   m_currentTime;

    };
} // namespace graphs