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
