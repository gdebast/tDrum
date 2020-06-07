#include "DrumUiTools/drumui_drumtabpartdrawerhelper.h"

#include "DrumEnum/drum_drumkit.h"
#include "DrumAPI/drum_drumexception.h"
#include "DrumAPI/drum_drumkithorizontallinehelper.h"
#include <DrumAPI/drum_drumline.h>

using namespace DrumUI;

DrumTabPartDrawerHelper::DrumTabPartDrawerHelper
    (const QRect& drawingArea, int maxPosition):
    m_drawingAreaQRect(drawingArea),
    m_drumTabPartGeometry(drawingArea.width(),drawingArea.height()),
    m_maxPosition(maxPosition)

{
    recompute(drawingArea,maxPosition);
}

const QRect &DrumTabPartDrawerHelper::getQRect(Drum::DrumKit drumKit, int position) const
{

    Drum::DrumKitHorizontalLineHelper helper;
    const auto thisKitLines = helper.getDrumKitHorizontalLines(drumKit);
    const auto thisLine = thisKitLines[0];

    // loop over the vector to find the rectangle which
    // can contain this drumkit at this position
    unsigned index(0);
    unsigned size(m_drumKitPositionQRect.size());
    while (index< size)
    {
        const auto& thisCombination = m_drumKitPositionQRect[index];
        if(std::get<0>(thisCombination) == thisLine
           && std::get<1>(thisCombination) == position)
        {
            return std::get<2>(thisCombination);
        }

        index++;
    }

    throw Drum::DrumException("Error from DrumTabPartDrawerHelper::getQRect: "
                              "did not found a valid rectangle for position" + std::to_string(position));

}

std::vector<Drum::DrumKit> DrumTabPartDrawerHelper::getDrumKits(int x, int y) const
{
    // loop over the vector to find the rectangle which
    // can contain this x-y  position
    unsigned index(0);
    unsigned size(m_drumKitPositionQRect.size());

    std::vector<Drum::DrumKit> returnedDrumKits;

    Drum::DrumKitHorizontalLineHelper helper;

    while (index< size)
    {
        const auto& thisCombination = m_drumKitPositionQRect[index];
        const auto& thisRect = std::get<2>(thisCombination);
        if (thisRect.left() < x && thisRect.right() > x
            && thisRect.top() < y && thisRect.bottom() > y)
        {
            for (auto kit : helper.getDrumKits(std::get<0>(thisCombination)))
                returnedDrumKits.push_back(kit);
        }
        index++;
    }

    return returnedDrumKits;

}

 const QRect* DrumTabPartDrawerHelper::getQRect(int x, int y) const
{
    // loop over the vector to find the rectangle which
    // can contain this x-y  position
    unsigned index(0);
    unsigned size(m_drumKitPositionQRect.size());

    std::vector<Drum::DrumKit> returnedDrumKits;

    while (index< size)
    {
        const auto& thisCombination = m_drumKitPositionQRect[index];
        const auto& thisRect = std::get<2>(thisCombination);
        if (thisRect.left() < x && thisRect.right() > x
            && thisRect.top() < y && thisRect.bottom() > y)
        {
            return &thisRect;
        }
        index++;
    }

    return nullptr;
 }

 Drum::DrumKitHorizontalLine DrumTabPartDrawerHelper::getDrumKitHorizontalLine(int x,
                                                                                       int y) const
 {

     for(const auto& combination : m_drumKitPositionQRect)
     {
         const auto& thisRect = std::get<2>(combination);
         if (thisRect.left() < x && thisRect.right() > x
             && thisRect.top() < y && thisRect.bottom() > y)
         {
             return std::get<0>(combination);
         }
     }

     return Drum::DrumKitHorizontalLine::None;
 }

 int DrumTabPartDrawerHelper::getPosition(int x, int y) const
 {
     for(const auto& combination : m_drumKitPositionQRect)
     {
         const auto& thisRect = std::get<2>(combination);
         if (thisRect.left() < x && thisRect.right() > x
             && thisRect.top() < y && thisRect.bottom() > y)
         {
             return std::get<1>(combination);
         }
     }

     return 0;
 }

 QRect DrumTabPartDrawerHelper::getDrumTabPartHorizontalLine(Drum::DrumKitHorizontalLine drumKitHorizontalLine) const
 {
     bool assertAsked(false);

     // all lines differ by the Y position
     auto x = m_drawingAreaQRect.x();
     auto width = m_drumTabPartGeometry.getWidth();
     auto height = m_drumTabPartGeometry.getHorizontalLineThickness();
     auto y = m_drawingAreaQRect.y();

     switch (drumKitHorizontalLine)
     {
     case Drum::DrumKitHorizontalLine::CrashLine:
         y += m_drumTabPartGeometry.getCrashLine();
         break;
     case Drum::DrumKitHorizontalLine::HiHatLine:
         y +=  m_drumTabPartGeometry.getHiHatLine();
         break;
     case Drum::DrumKitHorizontalLine::LowTomLine:
         assertAsked = true;
         break;
     case Drum::DrumKitHorizontalLine::HighTomLine:
         y += m_drumTabPartGeometry.getHighTomLine();
         break;
     case Drum::DrumKitHorizontalLine::BassDrumLine:
         y += m_drumTabPartGeometry.getBassDrumLine();
         break;
     case Drum::DrumKitHorizontalLine::FloorTomLine:
         y += m_drumTabPartGeometry.getFloorTomLine();
         break;
     case Drum::DrumKitHorizontalLine::SnareTomLine:
         y += m_drumTabPartGeometry.getSnareTomLine();
         break;
     case Drum::DrumKitHorizontalLine::HiHatFloorLine:
         y += m_drumTabPartGeometry.getHiHatFloorLine();
         break;
     case Drum::DrumKitHorizontalLine::None:
         assertAsked = true;
         break;
     }

     Drum::DrumException::drumAssert(assertAsked == false,"Error from DrumTabPartDrawerHelper::getDrumTabPartHorizontalLine: "
                                                          "requested a line impossible to draw");

     return QRect(x,y,width,height);

 }

 QRect DrumTabPartDrawerHelper::getDrumTabPartVerticalLine(bool leftLineIsAsked) const
 {
     if (leftLineIsAsked)
     {
         return QRect(m_drawingAreaQRect.x(),
                      m_drawingAreaQRect.y() + m_drumTabPartGeometry.getHiHatLine(),
                      m_drumTabPartGeometry.getVerticalLineThickness(),
                      m_drumTabPartGeometry.getBassDrumLine()-m_drumTabPartGeometry.getHiHatLine());
     }
     else
     {
         return QRect(m_drawingAreaQRect.x() + m_drumTabPartGeometry.getWidth()-(m_drumTabPartGeometry.getVerticalLineThickness()+1),
                      m_drawingAreaQRect.y() + m_drumTabPartGeometry.getHiHatLine(),
                      m_drumTabPartGeometry.getVerticalLineThickness(),
                      m_drumTabPartGeometry.getBassDrumLine()-m_drumTabPartGeometry.getHiHatLine());
     }
 }

 std::vector<QRect> DrumTabPartDrawerHelper::getDrumLine(const std::vector<Drum::DrumLine*>& drumLines) const
 {
     int largestPosition = m_maxPosition+2;
     std::vector<QRect> drumLineQRects;

     for(const auto* drumLine : drumLines)
     {

         int startPos(drumLine->getStartPosition());
         int endPos(drumLine->getEndPosition());

         // top line
         if(drumLine->isFloor() == false
         && drumLine->getDrumLineType() == Drum::DrumLine::DrumLineType::Croche)
         {
             int xRelativeStart = startPos*m_drumTabPartGeometry.getWidth()/largestPosition+m_drumTabPartGeometry.getDrumKitXsize() ;
             int xRelativeEnd = endPos*m_drumTabPartGeometry.getWidth()/largestPosition+m_drumTabPartGeometry.getDrumKitXsize()-1 ;
             drumLineQRects.push_back(QRect(xRelativeStart + m_drawingAreaQRect.x(),
                                            m_drumTabPartGeometry.getVerticalLineUpperEnd() + m_drawingAreaQRect.y(),
                                            xRelativeEnd-xRelativeStart,
                                            m_drumTabPartGeometry.getHorizontalCrocheLineThickness()));
         }
         // floor line
         else if(drumLine->isFloor() == true
         && drumLine->getDrumLineType() == Drum::DrumLine::DrumLineType::Croche)
         {
             int xRelativeStart = startPos*m_drumTabPartGeometry.getWidth()/largestPosition;
             int xRelativeEnd = endPos*m_drumTabPartGeometry.getWidth()/largestPosition-1;
             drumLineQRects.push_back(QRect(xRelativeStart + m_drawingAreaQRect.x(),
                                            m_drumTabPartGeometry.getVertivalLineBottomEnd()-m_drumTabPartGeometry.getHorizontalCrocheLineThickness() + m_drawingAreaQRect.y(),
                                            xRelativeEnd-xRelativeStart,
                                            m_drumTabPartGeometry.getHorizontalCrocheLineThickness()));
         }

     }

     return drumLineQRects;
 }

 QPolygon DrumTabPartDrawerHelper::getRepetitionMarker(bool isRepetitionEnd)
 {
     unsigned repWidth = m_drumTabPartGeometry.getRepetitionMarkerWidth();
     QPolygon polygon(3);
     QRect topLine = getDrumTabPartHorizontalLine(Drum::DrumKitHorizontalLine::HiHatLine);
     QRect middleLine = getDrumTabPartHorizontalLine(Drum::DrumKitHorizontalLine::SnareTomLine);
     QRect bottomLine = getDrumTabPartHorizontalLine(Drum::DrumKitHorizontalLine::BassDrumLine);

     // marker at the start of the drawing area
     if (isRepetitionEnd == false)
     {
         polygon.setPoint(0, m_drawingAreaQRect.left(),          topLine.top());
         polygon.setPoint(1, m_drawingAreaQRect.left()+repWidth, middleLine.top());
         polygon.setPoint(2, m_drawingAreaQRect.left(),          bottomLine.bottom());
     }
     // marker at the end of the drawing area
     else
     {
         polygon.setPoint(0,m_drawingAreaQRect.right(),          topLine.top());
         polygon.setPoint(1,m_drawingAreaQRect.right()-repWidth, middleLine.top());
         polygon.setPoint(2,m_drawingAreaQRect.right(),          bottomLine.bottom());
     }

     return polygon;
 }

void DrumTabPartDrawerHelper::recompute(const QRect& drawingArea,
                                                int maxPosition)
{
    m_drumTabPartGeometry.recompute(drawingArea.width(),drawingArea.height());
    m_drawingAreaQRect = drawingArea;
    m_maxPosition = maxPosition;

    int drumTabWidth = m_drumTabPartGeometry.getWidth();
    int largestPosition = maxPosition+2;
    int rectWidth = m_drumTabPartGeometry.getDrumKitXsize();
    int rectHeight = m_drumTabPartGeometry.getLineSpacing();

    m_drumKitPositionQRect = {};

    for(int positionInPart(1); positionInPart <= maxPosition; positionInPart++)
    {
        // compute the upper left corner X of the rectangle
        // (common to every drum kit at this position)
        int rectUpperLeftCornerX = positionInPart*drumTabWidth/largestPosition
                                   + m_drawingAreaQRect.x(); // shifted

        // add Rect-position-dumKit combination
        // ------------------------------------
        // in this logic, each drum kit can be in a specific rectangle
        // for each position. The silence can be at every position.

        // make the crash rectangle : crah, ride and ride-bell
        QRect rect(rectUpperLeftCornerX,
                   m_drumTabPartGeometry.getCrashLine()-rectHeight + m_drawingAreaQRect.y(),
                   rectWidth,rectHeight);
        m_drumKitPositionQRect.push_back(std::make_tuple(Drum::DrumKitHorizontalLine::CrashLine,
                                                         positionInPart,
                                                         rect));


        // make the hihat rectangle
        rect = QRect(rectUpperLeftCornerX,
                     m_drumTabPartGeometry.getHiHatLine()-rectHeight + m_drawingAreaQRect.y(),
                     rectWidth,rectHeight);
        m_drumKitPositionQRect.push_back(std::make_tuple(Drum::DrumKitHorizontalLine::HiHatLine,
                                                         positionInPart,
                                                         rect));

        // make the snare rectangle
        // NOTE : is important to respect the sequence snare->low->high tom
        //        indeed, the rectangle should have this order of priority
        rect = QRect(rectUpperLeftCornerX,
                     m_drumTabPartGeometry.getSnareTomLine()-rectHeight + m_drawingAreaQRect.y(),
                     rectWidth,rectHeight);
        m_drumKitPositionQRect.push_back(std::make_tuple(Drum::DrumKitHorizontalLine::SnareTomLine,
                                                         positionInPart,
                                                         rect));

        // make the low tom rectangle
        rect = QRect(rectUpperLeftCornerX,
                     m_drumTabPartGeometry.getSnareTomLine()-rectHeight/2.0 + m_drawingAreaQRect.y(),
                     rectWidth,rectHeight);
        m_drumKitPositionQRect.push_back(std::make_tuple(Drum::DrumKitHorizontalLine::LowTomLine,
                                                         positionInPart,
                                                         rect));


        // make the high tom rectangle
        rect = QRect(rectUpperLeftCornerX,
                     m_drumTabPartGeometry.getHighTomLine()-rectHeight + m_drawingAreaQRect.y(),
                     rectWidth,rectHeight);
        m_drumKitPositionQRect.push_back(std::make_tuple(Drum::DrumKitHorizontalLine::HighTomLine,
                                                         positionInPart,
                                                         rect));


        // make the floor tom rectangle
        rect = QRect(rectUpperLeftCornerX,
                     m_drumTabPartGeometry.getFloorTomLine()-rectHeight + m_drawingAreaQRect.y(),
                     rectWidth,rectHeight);
        m_drumKitPositionQRect.push_back(std::make_tuple(Drum::DrumKitHorizontalLine::FloorTomLine,
                                                         positionInPart,
                                                         rect));

        // make the bass drum rectangle
        rect = QRect(rectUpperLeftCornerX,
                     m_drumTabPartGeometry.getBassDrumLine()-rectHeight + m_drawingAreaQRect.y(),
                     rectWidth,rectHeight);
        m_drumKitPositionQRect.push_back(std::make_tuple(Drum::DrumKitHorizontalLine::BassDrumLine,
                                                         positionInPart,
                                                         rect));

        // make the hi hat floor rectangle
        rect = QRect(rectUpperLeftCornerX,
                     m_drumTabPartGeometry.getHiHatFloorLine()-rectHeight + m_drawingAreaQRect.y(),
                     rectWidth,rectHeight);
        m_drumKitPositionQRect.push_back(std::make_tuple(Drum::DrumKitHorizontalLine::HiHatFloorLine,
                                                         positionInPart,
                                                         rect));

    }
}
