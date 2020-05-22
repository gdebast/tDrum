#include "DrumWidget/drumui_drumtabpartcreatorwidget.h"

#include "DrumEnum/drum_drumkithorizontalline.h"
#include "DrumAPI/drum_drumtabpart.h"
#include "DrumAPI/drum_drumkithorizontallinehelper.h"

#include <QMouseEvent>
#include <QRect>
#include <QPainter>

using namespace DrumUI;

DrumTabPartCreatorWidget::DrumTabPartCreatorWidget(Drum::DrumTabPart* drumTabPartModel,
                                                   QWidget *parent) :
    DrumTabPartWidgetBase(drumTabPartModel,parent)
{
    // this widget tracked the mouse to enable fine Qrect drawing
    // see method paintHooveredSelectedQRect
    setMouseTracking(true);
}

// =========================
// == protected functions ==
// =========================
void DrumTabPartCreatorWidget::paintEvent(QPaintEvent *e)
{
    paintHooveredSelectedQRect();

    DrumTabPartWidgetBase::paintEvent(e);
}

QSize DrumTabPartCreatorWidget::sizeHint() const
{
    // this widget should be a bit bigger
    float biggerFactor = 1.5;
    return QSize(DrumTabPartWidgetBase::getFixedWidth()*biggerFactor,
                 DrumTabPartWidgetBase::getFixedHeight()*biggerFactor);
}

void DrumTabPartCreatorWidget::mouseMoveEvent(QMouseEvent *event)
{

    // avoid the over-flow of memory
    if (m_hooveredSelectedQRect)
    {
        delete m_hooveredSelectedQRect;
        m_hooveredSelectedQRect = nullptr;
    }

    // find the rectangle under the mouse
    const QRect *rect{nullptr};
    if (event)
    {
        auto eventX = event->pos().x();
        auto eventY = event->pos().y();
        rect = m_drumTabPartDrawerHelper.getQRect(eventX,
                                                          eventY);
    }

    if (rect)
    {
        // a copy is needed to not change the QRect in DrumKitPositionDrawingRectangle
        m_hooveredSelectedQRect = new QRect(*rect);
    }
    else
    {
        m_hooveredSelectedQRect = nullptr;
    }

    // repaint because the hoovered rectangle changed
    repaint();
}

void DrumTabPartCreatorWidget::mousePressEvent(QMouseEvent *event)
{

    auto eventX = event->x();
    auto eventY = event->y();

    if(event->button() == Qt::LeftButton)
    {
        leftMouseClickEvent(eventX, eventY);
    }
    else if (event->button() == Qt::RightButton)
    {
        DrumTabPartWidgetBase::rightMouseClickEvent(eventX, eventY);
    }

    repaint();

}

// =======================
// == private functions ==
// =======================

void DrumTabPartCreatorWidget::leftMouseClickEvent(int eventX, int eventY)
{
    auto line = m_drumTabPartDrawerHelper.getDrumKitHorizontalLine(eventX, eventY);
    // do nothing if there is no rect
    if (line == Drum::DrumKitHorizontalLine::None)
        return;

    // find the position
    auto position = m_drumTabPartDrawerHelper.getPosition(eventX,eventY);
    if (position == 0)
        return;

    // find the possible DrumKits at this line
    Drum::DrumKitHorizontalLineHelper helper;
    auto possibleNewDrumKits = helper.getDrumKits(line);
    if (possibleNewDrumKits.size() == 0)
        return;

    // find the current Drumkit
    auto currentDrumKit = m_drumTabPartModel->getDrumKit(line,position);

    // loop over the possible new drumkit and choose the new one
    unsigned index(0);
    unsigned size(possibleNewDrumKits.size());
    bool found(false);
    while (index < size && found == false)
    {
        found = currentDrumKit == possibleNewDrumKits[index];
        index++;
    }
    // take the next one
    Drum::DrumKit newDrumKit(Drum::DrumKit::Silence); // default
    if(index < size)
        newDrumKit = possibleNewDrumKits[index];
    else if (index == size)
        newDrumKit = possibleNewDrumKits[0];


    // edit the model
    if (newDrumKit == Drum::DrumKit::Silence)
    {
        m_drumTabPartModel->clearLineAtPosition(line,position);
    }
    else
    {
        m_drumTabPartModel->addDrumKit(newDrumKit,position);
    }

}

void DrumTabPartCreatorWidget::paintHooveredSelectedQRect()
{

    QPainter p(this);

    // colors
    QColor selectedBlue = QColor(180,200,255);

    // mousse hover or selected state of the widget
    if(m_hooveredSelectedQRect)
        p.fillRect(*m_hooveredSelectedQRect,selectedBlue);
    else
        p.fillRect(QRect(0,0,width(),height()),Qt::transparent);

}
