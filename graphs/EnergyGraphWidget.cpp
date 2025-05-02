#include "EnergyGraphWidget.h"


namespace graphs
{
    EnergyGraphWidget::EnergyGraphWidget(QWidget *parent)
        : QWidget(parent),
        m_kineticSeries(new QLineSeries(this)),
        m_potentialSeries(new QLineSeries(this)),
        m_totalSeries(new QLineSeries(this)),
        m_chart(new QChart()),
        m_axisX(new QValueAxis()),
        m_axisY(new QValueAxis()),
      m_timer(new QTimer(this)),
      m_currentTime(0.0)
{
    // Label the series
    m_kineticSeries->setName("Kinetic Energy");
    // m_potentialSeries->setName("Potential Energy");
    // m_totalSeries->setName("Total Energy");

    // Configure and assemble chart
    m_chart->addSeries(m_kineticSeries);
    // m_chart->addSeries(m_potentialSeries);
    // m_chart->addSeries(m_totalSeries);

    m_axisX->setTitleText("Time (s)");
    m_axisX->setLabelFormat("%.1f");
    m_axisX->setRange(0.0, 10.0);
    m_chart->addAxis(m_axisX, Qt::AlignBottom);

    m_axisY->setTitleText("Energy");
    m_axisY->setRange(0.0, 150.0);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);

    // Attach axes to each series
    m_kineticSeries->attachAxis(m_axisX);
    // m_potentialSeries->attachAxis(m_axisX);
    // m_totalSeries->attachAxis(m_axisX);
    m_kineticSeries->attachAxis(m_axisY);
    // m_potentialSeries->attachAxis(m_axisY);
    // m_totalSeries->attachAxis(m_axisY);

    // Chart view inside the widget
    QChartView *chartView = new QChartView(m_chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
    setLayout(layout);


}

void EnergyGraphWidget::addDataPoint(qreal time, qreal kinetic, qreal potential)
{
    // Append points to each series
    m_kineticSeries->append(time, kinetic);
    m_potentialSeries->append(time, potential);
    m_totalSeries->append(time, kinetic + potential);

    // Scroll X‑axis once we move past its current upper bound
    if (time > m_axisX->max())
    {
        m_axisX->setRange(m_axisX->min() + 1.0, time + 1.0);
    }

    // Auto‑adjust Y‑axis if energies exceed the visible range
    qreal maxVisible = qMax(qMax(kinetic, potential), kinetic + potential);
    if (maxVisible > m_axisY->max())
    {
        m_axisY->setMax(maxVisible + 10.0);
    }
}
} // namespace graphs

