#pragma once

#include "DrumEnum/drum_drumkit.h"

#include <QPaintDevice>
#include <QPainter>

namespace  Drum
{
    class DrumTabPart;
}

namespace  DrumUI
{
    class DrumTabPartDrawerHelper;
}

namespace DrumUI
{

    class DrumTabPartDrawer
    {
        /*
         * class responsible for drawing a drum tab part on a QPaintDevice
         * This paint device can be a QWidget.
         */

        public:
            explicit DrumTabPartDrawer(QPaintDevice& deviceToPaint);
            DrumTabPartDrawer(const DrumTabPartDrawer&) = delete;
            DrumTabPartDrawer(DrumTabPartDrawer&&) = delete;
            DrumTabPartDrawer& operator=(const DrumTabPartDrawer&) = delete;
            DrumTabPartDrawer& operator=(DrumTabPartDrawer&&) = delete;
            ~DrumTabPartDrawer() = default;

            void DrawDrumTabPart(Drum::DrumTabPart& drumTabPart,
                                 const DrumTabPartDrawerHelper& helper,
                                 QPainter& painter) const;
            void DrawDrumTabPart(Drum::DrumTabPart& drumTabPart,
                                 const DrumTabPartDrawerHelper& helper) const;

            /**
             * @brief draw a rectangle which indicates an implicit repetition.
             *        The rectangle indicates that a number of identical parts
             *        of the drum tab should be repeated and the part size.
             * @param drawingArea, drawing area
             * @param painter, QPainter used to draw the shapes
             * @param identicalPartNumber, number of time the part is repeated
             */
            void drawRepetion(const QRect& drawingArea,
                              QPainter& painter,
                              unsigned identicalPartNumber);

            /**
             * @brief add a mark to the right or the left of the drawing area.
             *        This marks the main repetition.
             * @param drawingArea, rectangle where to draw left or right
             * @param painter, QPainter used to draw the shapes
             * @param isEndOfRepetition, bool indicating if the mark should be on the left (false) or the right (true)
             */
            void markRepeatedPart(const QRect& drawingArea,
                                  QPainter& painter,
                                  bool isEndOfRepetition);


        private:

            // drawing functions
            void drawTabLines(const DrumTabPartDrawerHelper &helper, QPainter& painter) const;
            void drawDrumKits(const Drum::DrumTabPart& drumTabPart, const DrumTabPartDrawerHelper &helper, QPainter& painter) const;
            void drawDrumKit(Drum::DrumKit drumKit, const QRect &rectWhereToDraw,const DrumTabPartDrawerHelper& helper, QPainter& painter) const;
            void drawCymbal(int xCenter, int line, const DrumTabPartDrawerHelper& helper, QPainter& painter) const;
            void drawCircledCymbal(int xCenter, int line, const DrumTabPartDrawerHelper& helper, QPainter& painter) const;
            void drawUnderscoredCircledCymbal(int xCenter, int line, const DrumTabPartDrawerHelper& helper, QPainter& painter) const;
            void drawUnderscoredCymbal(int xCenter, int line,const DrumTabPartDrawerHelper& helper, QPainter& painter) const;
            void drawTom(int xCenter, int line, const DrumTabPartDrawerHelper& helper, QPainter& painter) const;
            void drawDoubleTom(int xCenter, int line, const DrumTabPartDrawerHelper& helper, QPainter& painter) const;
            void drawBell(int xCenter, int line, const DrumTabPartDrawerHelper& helper, QPainter& painter) const;
            void drawUnderscoredBell(int xCenter, int line, const DrumTabPartDrawerHelper& helper, QPainter& painter) const;
            void drawBottomLine(int xCenter, int line, const DrumTabPartDrawerHelper& helper, QPainter& painter, bool shift = true) const;
            void drawUpperLine(int xCenter, int line, const DrumTabPartDrawerHelper& helper, QPainter& painter, bool shift = true) const;
            void drawDrumLines(Drum::DrumTabPart& drumTabPart, const DrumTabPartDrawerHelper& helper, QPainter& painter) const;


            // members
            QPaintDevice&      m_deviceToPaint;

    };

}
