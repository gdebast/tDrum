#include "DrumWidget/drumui_drumtabrownumberwidget.h"

#include "DrumUiTools/drumui_drumtabpartgeometry.h"

#include <QPainter>

using namespace DrumUI;

DrumTabRowNumberWidget::DrumTabRowNumberWidget(unsigned displayedRowNumber, double zoomFactor , QWidget *parent) :
    QWidget(parent), m_displayedRowNumber(displayedRowNumber), m_zoomFactor(zoomFactor)
{

}

void DrumTabRowNumberWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    // draw the number in the middle
    QFont font;
    font.setPointSize(m_zoomFactor*12);
    QPainter painter(this);
    painter.setFont(font);
    painter.drawText(3*width()/4,height()/2,QString(std::to_string(m_displayedRowNumber).c_str()));

}

QSize DrumTabRowNumberWidget::sizeHint() const
{
    return QSize(m_zoomFactor*DrumTabPartGeometry::getBaseWidght(), m_zoomFactor*DrumTabPartGeometry::getBaseHeight());
}
