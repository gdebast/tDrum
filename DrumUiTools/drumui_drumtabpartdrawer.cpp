#include "DrumUiTools/drumui_drumtabpartdrawer.h"

#include "DrumUiTools/drumui_drumtabpartdrawerhelper.h"
#include <DrumAPI/drum_drumtabpart.h>

using namespace DrumUI;


DrumTabPartDrawer::DrumTabPartDrawer(QPaintDevice &deviceToPaint,
                                     Drum::DrumTabPart &drumTabPart) :
    m_deviceToPaint(deviceToPaint),
    m_drumTabPart(drumTabPart)
{
}

void DrumTabPartDrawer::DrawDrumTabPart(const DrumTabPartDrawerHelper &helper,
                                        QPainter &painter) const
{
    // draw lines
    drawTabLines(helper,painter);

    // draw the drum kits
    drawDrumKits(helper,painter);

    // draw the horizontal lines linking the kits
    drawDrumLines(helper,painter);
}

void DrumTabPartDrawer::DrawDrumTabPart(const DrumTabPartDrawerHelper &helper) const
{
    QPainter commonPainter(&m_deviceToPaint);
    DrawDrumTabPart(helper,commonPainter);

}

void DrumTabPartDrawer::drawRepetion(const DrumTabPartDrawerHelper &helper,
                                     QPainter &painter,
                                     unsigned identicalPartNumber,
                                     unsigned partSize)
{
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);

    // draw the big-rectangle
    painter.drawRect(helper.getDrumTabPartHorizontalLine(Drum::DrumKitHorizontalLine::HiHatLine));
    painter.drawRect(helper.getDrumTabPartHorizontalLine(Drum::DrumKitHorizontalLine::BassDrumLine));
    painter.drawRect(helper.getDrumTabPartVerticalLine());
    painter.drawRect(helper.getDrumTabPartVerticalLine(true));

    // TODO: draw inner-rectangles (# = identicalPartNumber) with the number of repeted parts (partSize)
}

// ===============================
// == private drawing functions ==
// ===============================

void DrumTabPartDrawer::drawTabLines(const DrumTabPartDrawerHelper &helper,
                                     QPainter& painter) const
{

    // draw the 5 horizontal lines
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawRect(helper.getDrumTabPartHorizontalLine(Drum::DrumKitHorizontalLine::HiHatLine));
    painter.drawRect(helper.getDrumTabPartHorizontalLine(Drum::DrumKitHorizontalLine::HighTomLine));
    painter.drawRect(helper.getDrumTabPartHorizontalLine(Drum::DrumKitHorizontalLine::SnareTomLine));
    painter.drawRect(helper.getDrumTabPartHorizontalLine(Drum::DrumKitHorizontalLine::FloorTomLine));
    painter.drawRect(helper.getDrumTabPartHorizontalLine(Drum::DrumKitHorizontalLine::BassDrumLine));
    // draw two vertical lines to separated the drum tab parts
    painter.drawRect(helper.getDrumTabPartVerticalLine());
    painter.drawRect(helper.getDrumTabPartVerticalLine(true));


}

void DrumTabPartDrawer::drawDrumKits(const DrumTabPartDrawerHelper &helper,
                                     QPainter& painter) const
{

    auto allDrumKitPosition = m_drumTabPart.getDrumKits();
    for(auto kitPosition : allDrumKitPosition)
    {
        // get the position of this drum kit in the DrumTabPart
        int position = static_cast<int>(kitPosition.first);
        // get the rectangle when the drum kit is at that position
        const auto& rect = helper.getQRect(kitPosition.second,position);
        // draw the drumKit at this position
        drawDrumKit(kitPosition.second, rect, helper, painter);
    }
}

void DrumTabPartDrawer::drawTom(int xCenter,
                                int line,
                                const DrumTabPartDrawerHelper &helper,
                                QPainter& painter) const
{
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    const auto& geo = helper.getDrumTabPartGeometry();

    painter.drawEllipse(QPoint(xCenter,
                               line-geo.getLineSpacing()/2),
                        geo.getDrumKitXsize()/2,
                        geo.getLineSpacing()/2);

}

void DrumTabPartDrawer::drawCymbal(int xCenter,
                                   int line,
                                   const DrumTabPartDrawerHelper &helper,
                                   QPainter& painter) const
{
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    const auto& geo = helper.getDrumTabPartGeometry();

    // draw the rect from up to bottom
    int x1 = xCenter-geo.getDrumKitXsize()/2;
    int y1 = line-geo.getLineSpacing();
    int x2 = xCenter+geo.getDrumKitXsize()/2;
    int y2 = line;
    QPointF point1s[4] =
    {
        QPointF(x1,y1+geo.getCymbalYThickness()),
        QPointF(x1+geo.getCymbalXThickness(),y1),
        QPointF(x2,y2-geo.getCymbalYThickness()),
        QPointF(x2-geo.getCymbalXThickness(),y2)
    };
    painter.drawConvexPolygon(point1s,4);

    // draw the line from bottom to up. the / of the cross
    x1 = xCenter-geo.getDrumKitXsize()/2;
    y1 = line;
    x2 = xCenter+geo.getDrumKitXsize()/2;
    y2 = line-geo.getLineSpacing();
    QPointF point2s[4] =
    {
        QPointF(x1+geo.getCymbalXThickness(),y1),
        QPointF(x1,y1-geo.getCymbalYThickness()),
        QPointF(x2-geo.getCymbalXThickness(),y2),
        QPointF(x2,y2+geo.getCymbalYThickness())
    };
    painter.drawConvexPolygon(point2s,4);

}

void DrumTabPartDrawer::drawUnderscoredCymbal(int xCenter,
                                              int line,
                                              const DrumTabPartDrawerHelper &helper,
                                              QPainter& painter) const
{
    drawCymbal(xCenter, line, helper, painter);

    const auto& geo = helper.getDrumTabPartGeometry();
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawRect(xCenter-geo.getDrumKitXsize()/2,
                     line,
                     geo.getDrumKitXsize(),
                     geo.getHorizontalLineThickness());


}

void DrumTabPartDrawer::drawCircledCymbal(int xCenter,
                                          int line,
                                          const DrumTabPartDrawerHelper &helper,
                                          QPainter& painter) const
{


    const auto& geo = helper.getDrumTabPartGeometry();
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawEllipse(QPoint(xCenter,line-geo.getLineSpacing()/2),
                        geo.getDrumKitXsize()/2,
                        geo.getLineSpacing()/2);

    painter.setPen(Qt::black);
    painter.setBrush(Qt::white);
    painter.drawEllipse(QPoint(xCenter,line-geo.getLineSpacing()/2),
                        geo.getDrumKitXsize()/2-geo.getCymbalXThickness(),
                        geo.getLineSpacing()/2-geo.getCymbalXThickness());

    drawCymbal(xCenter, line, helper, painter);
}

void DrumTabPartDrawer::drawUnderscoredCircledCymbal(int xCenter,
                                                     int line,
                                                     const DrumTabPartDrawerHelper &helper,
                                                     QPainter& painter) const
{
    drawCircledCymbal(xCenter, line, helper, painter);

    const auto& geo = helper.getDrumTabPartGeometry();
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawRect(xCenter-geo.getDrumKitXsize()/2,
                     line,
                     geo.getDrumKitXsize(),
                     geo.getHorizontalLineThickness());
}

void DrumTabPartDrawer::drawBell(int xCenter,
                                 int line,
                                 const DrumTabPartDrawerHelper &helper,
                                 QPainter& painter) const
{
    painter.setPen(Qt::black);
    const auto& geo = helper.getDrumTabPartGeometry();

    // outer diamond
    painter.setBrush(Qt::black);
    QPointF point1s[4] =
    {
        QPointF(xCenter,line),
        QPointF(xCenter-geo.getDrumKitXsize()/2,line-geo.getLineSpacing()/2),
        QPointF(xCenter,line-geo.getLineSpacing()),
        QPointF(xCenter+geo.getDrumKitXsize()/2,line-geo.getLineSpacing()/2)
    };
    painter.drawConvexPolygon(point1s,4);

    // inner diamond
    painter.setBrush(Qt::white);
    QPointF point2s[4] =
    {
        QPointF(xCenter,line-geo.getCymbalXThickness()),
        QPointF(xCenter-geo.getDrumKitXsize()/2+geo.getCymbalXThickness(),line-geo.getLineSpacing()/2),
        QPointF(xCenter,line-geo.getLineSpacing()+geo.getCymbalXThickness()),
        QPointF(xCenter+geo.getDrumKitXsize()/2-geo.getCymbalXThickness(),line-geo.getLineSpacing()/2)
    };
    painter.drawConvexPolygon(point2s,4);
}

void DrumTabPartDrawer::drawUnderscoredBell(int xCenter,
                                            int line,
                                            const DrumTabPartDrawerHelper &helper,
                                            QPainter& painter) const
{
    drawBell(xCenter,line,helper,painter);

    const auto& geo = helper.getDrumTabPartGeometry();
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawRect(xCenter-geo.getDrumKitXsize()/2,
               line,
               geo.getDrumKitXsize(),
               geo.getHorizontalLineThickness());
}

void DrumTabPartDrawer::drawBottomLine(int xCenter,
                                       int line,
                                       const DrumTabPartDrawerHelper &helper,
                                       QPainter& painter,
                                       bool shift) const
{
    const auto& geo = helper.getDrumTabPartGeometry();
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);

    int lineShift(0);
    if(shift)
        lineShift = geo.getLineSpacing()/2;

    int lineStart = line - lineShift;
    painter.drawRect(xCenter-geo.getDrumKitXsize()/2,
                     lineStart,
                     geo.getVerticalLineThickness(),
                     geo.getVertivalLineBottomEnd() - lineStart + helper.getDrawingArea().y());
}

void DrumTabPartDrawer::drawUpperLine(int xCenter,
                                      int line,
                                      const DrumTabPartDrawerHelper &helper,
                                      QPainter& painter,
                                      bool shift) const
{
    const auto& geo = helper.getDrumTabPartGeometry();
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);

    int lineShift(0);
    if(shift)
        lineShift = geo.getLineSpacing()/2;
    else
        lineShift = geo.getLineSpacing();

    int lineStart = geo.getVerticalLineUpperEnd() + helper.getDrawingArea().y();
    painter.drawRect(xCenter+geo.getDrumKitXsize()/2,
                     lineStart,
                     geo.getVerticalLineThickness(),
                     (line - lineShift) - lineStart);

}

void DrumTabPartDrawer::drawDrumKit(Drum::DrumKit drumKit,
                                    const QRect &rectWhereToDraw,
                                    const DrumTabPartDrawerHelper& helper,
                                    QPainter& painter) const
{
    int xCenter = rectWhereToDraw.center().x();

    switch (drumKit) {
    case Drum::DrumKit::Silence :
        break;
    case Drum::DrumKit::Ride :
        drawUnderscoredCymbal(xCenter,rectWhereToDraw.bottom(),helper,painter);
        drawUpperLine(xCenter,rectWhereToDraw.bottom(),helper,painter,false);
        break;
    case Drum::DrumKit::Crash :
        drawUnderscoredCircledCymbal(xCenter,rectWhereToDraw.bottom(),helper,painter);
        drawUpperLine(xCenter,rectWhereToDraw.bottom(),helper,painter,false);
        break;
    case Drum::DrumKit::LowTom :
        drawTom(xCenter,rectWhereToDraw.bottom(),helper,painter);
        drawUpperLine(xCenter,rectWhereToDraw.bottom(),helper,painter);
        break;
    case Drum::DrumKit::HighTom :
        drawTom(xCenter,rectWhereToDraw.bottom(),helper,painter);
        drawUpperLine(xCenter,rectWhereToDraw.bottom(),helper,painter);
        break;
    case Drum::DrumKit::BassDrum :
        drawTom(xCenter,rectWhereToDraw.bottom(),helper,painter);
        drawBottomLine(xCenter,rectWhereToDraw.bottom(),helper,painter);
        break;
    case Drum::DrumKit::FloorTom :
        drawTom(xCenter,rectWhereToDraw.bottom(),helper,painter);
        drawUpperLine(xCenter,rectWhereToDraw.bottom(),helper,painter);
        break;
    case Drum::DrumKit::SnareTom :
        drawTom(xCenter,rectWhereToDraw.bottom(),helper,painter);
        drawUpperLine(xCenter,rectWhereToDraw.bottom(),helper,painter);
        break;
    case Drum::DrumKit::HiHatClosed :
        drawCymbal(xCenter,rectWhereToDraw.bottom(),helper,painter);
        drawUpperLine(xCenter,rectWhereToDraw.bottom(),helper,painter,false);
        break;
    case Drum::DrumKit::HiHatOpened :
        drawCircledCymbal(xCenter,rectWhereToDraw.bottom(),helper,painter);
        drawUpperLine(xCenter,rectWhereToDraw.bottom(),helper,painter,false);
        break;
    case Drum::DrumKit::RideBell :
        drawUnderscoredBell(xCenter,rectWhereToDraw.bottom(),helper,painter);
        drawUpperLine(xCenter,rectWhereToDraw.bottom(),helper,painter);
        break;
    case Drum::DrumKit::HitHatFloor :
        drawCymbal(xCenter,rectWhereToDraw.bottom(),helper,painter);
        drawBottomLine(xCenter,rectWhereToDraw.bottom(),helper,painter);
        break;
    }

}

void DrumTabPartDrawer::drawDrumLines(const DrumTabPartDrawerHelper &helper,
                                      QPainter& painter) const
{

    // painter and geometry object
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);

    auto drumLineQRects = helper.getDrumLine(m_drumTabPart.getDrumLines());

    for(const auto& Rect : drumLineQRects)
    {
        painter.drawRect(Rect);
    }

}

