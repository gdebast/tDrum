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
            explicit DrumTabPartDrawer(QPaintDevice& deviceToPaint,
                                       Drum::DrumTabPart& drumTabPart);
            DrumTabPartDrawer(const DrumTabPartDrawer&) = delete;
            DrumTabPartDrawer(DrumTabPartDrawer&&) = delete;
            DrumTabPartDrawer& operator=(const DrumTabPartDrawer&) = delete;
            DrumTabPartDrawer() = delete;
            ~DrumTabPartDrawer() = default;

            void DrawDrumTabPart(const DrumTabPartDrawerHelper& helper, QPainter& painter) const;
            void DrawDrumTabPart(const DrumTabPartDrawerHelper& helper) const;

            /**
             * @brief draw a rectangle which indicates an implicit repetition.
             *        The rectangle indicates that a number of identical parts
             *        of the drum tab should be repeated and the part size.
             * @param helper, the object helping to draw by giving the drawing area
             * @param painter, QPainter used to draw the shapes
             * @param identicalPartNumber, number of time the part is repeated
             */
            void drawRepetion(const DrumTabPartDrawerHelper& helper,
                              QPainter& painter,
                              unsigned identicalPartNumber);

        private:

            // drawing functions
            void drawTabLines(const DrumTabPartDrawerHelper &helper, QPainter& painter) const;
            void drawDrumKits(const DrumTabPartDrawerHelper &helper, QPainter& painter) const;
            void drawDrumKit(Drum::DrumKit drumKit, const QRect &rectWhereToDraw,const DrumTabPartDrawerHelper& helper, QPainter& painter) const;
            void drawCymbal(int xCenter, int line, const DrumTabPartDrawerHelper& helper, QPainter& painter) const;
            void drawCircledCymbal(int xCenter, int line, const DrumTabPartDrawerHelper& helper, QPainter& painter) const;
            void drawUnderscoredCircledCymbal(int xCenter, int line, const DrumTabPartDrawerHelper& helper, QPainter& painter) const;
            void drawUnderscoredCymbal(int xCenter, int line,const DrumTabPartDrawerHelper& helper, QPainter& painter) const;
            void drawTom(int xCenter, int line, const DrumTabPartDrawerHelper& helper, QPainter& painter) const;
            void drawBell(int xCenter, int line, const DrumTabPartDrawerHelper& helper, QPainter& painter) const;
            void drawUnderscoredBell(int xCenter, int line, const DrumTabPartDrawerHelper& helper, QPainter& painter) const;
            void drawBottomLine(int xCenter, int line, const DrumTabPartDrawerHelper& helper, QPainter& painter, bool shift = true) const;
            void drawUpperLine(int xCenter, int line, const DrumTabPartDrawerHelper& helper, QPainter& painter, bool shift = true) const;
            void drawDrumLines(const DrumTabPartDrawerHelper& helper, QPainter& painter) const;

            // members
            QPaintDevice&      m_deviceToPaint;
            Drum::DrumTabPart& m_drumTabPart; // non-const because it can be modified when asking the drum lines

    };

}
