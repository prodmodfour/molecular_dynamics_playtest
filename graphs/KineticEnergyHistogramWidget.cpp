#include "KineticEnergyHistogramWidget.h"

#include <QPainter>
#include <algorithm>
#include <cmath>
#include <numeric>

KineticEnergyHistogramWidget::KineticEnergyHistogramWidget(QWidget* parent)
    : QWidget(parent),
      m_binCount(20),
      m_minEnergy(0.0),
      m_maxEnergy(1.0)
{
    setMinimumSize(200, 150);
}

void KineticEnergyHistogramWidget::setEnergies(const std::vector<double>& energies)
{
    m_data = energies;
    recomputeBins();
    update();
}

void KineticEnergyHistogramWidget::setBinCount(int bins)
{
    if (bins > 0 && bins != m_binCount) {
        m_binCount = bins;
        recomputeBins();
        update();
    }
}

void KineticEnergyHistogramWidget::paintEvent(QPaintEvent* /*event*/)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const int w = width();
    const int h = height();
    const int padding = 35;                

    if (m_binFrequencies.empty() ||
        std::accumulate(m_binFrequencies.begin(), m_binFrequencies.end(), 0) == 0) {
        p.drawText(rect(), Qt::AlignCenter, "No data");
        return;
    }

    const double barWidth = static_cast<double>(w - padding) / m_binCount;
    const int    maxFreq  = *std::max_element(m_binFrequencies.begin(),
                                              m_binFrequencies.end());

    // ---- Axes -------------------------------------------------------------
    p.setPen(Qt::black);
    p.drawLine(padding, h - padding, w, h - padding);   
    p.drawLine(padding, padding, padding, h - padding); 

    // ---- Histogram bars ---------------------------------------------------
    for (int i = 0; i < m_binCount; ++i) {
        const double freq      = m_binFrequencies[i];
        const double barHeight = static_cast<double>(h - 2 * padding) * freq / maxFreq;

        QRectF barRect(padding + i * barWidth + 1,
                       h - padding - barHeight,
                       barWidth - 2,
                       barHeight);

        p.fillRect(barRect, Qt::blue);
        p.drawRect(barRect);
    }

    // ---- Tick labels ------------------------------------------------------
    p.setFont(QFont("Sans", 8));
    for (int i = 0; i <= m_binCount; ++i) {
        const double energy = m_minEnergy +
                              i * (m_maxEnergy - m_minEnergy) / m_binCount;

        const int x = static_cast<int>(padding + i * barWidth);
        const QString label = QString::number(energy, 'g', 3);

        p.drawText(x - 10, h - padding + 15, label);
    }
}

void KineticEnergyHistogramWidget::recomputeBins()
{
    m_binFrequencies.assign(m_binCount, 0);

    if (m_data.empty()) {
        m_minEnergy = m_maxEnergy = 0.0;
        return;
    }

    const auto [minIt, maxIt] = std::minmax_element(m_data.begin(), m_data.end());
    m_minEnergy = *minIt;
    m_maxEnergy = *maxIt;

    if (m_minEnergy == m_maxEnergy) {

        m_binFrequencies[m_binCount / 2] = static_cast<int>(m_data.size());
        return;
    }

    const double binSpan = (m_maxEnergy - m_minEnergy) / m_binCount;

    for (double e : m_data) {
        int idx = static_cast<int>(std::floor((e - m_minEnergy) / binSpan));
        if (idx == m_binCount) idx -= 1;   
        ++m_binFrequencies[idx];
    }
}
