#ifndef DRUMKITPOSITIONDRAWINGRECTANGLE_H
#define DRUMKITPOSITIONDRAWINGRECTANGLE_H

#include "DrumEnum/drum_drumkithorizontalline.h"
#include "DrumEnum/drum_drumkit.h"
#include "DrumUiTools/drumui_drumtabpartgeometry.h"

#include <QRect>

#include <vector>
#include <tuple>

namespace Drum {
    class DrumLine;
}

namespace DrumUI {

/*
 * This class helps DrumTabPartDrawer by giving some QRect to draw:
 * 1. the possible drumkit-position-rectangle combination.
 *    The position is the integer from 1 to the drum tab part time
 *    The rectangle is the area of the widget which can have this DrumKit
 * 2. The drawing area which is a rectangle where to draw.
 * 3. The vertical and horizontal lines of a drum tab part
 * 4. The DrumLine QRect to tie the DrumKit in croche and double-croche
 */

    class DrumTabPartDrawerHelper
    {
    public:
        // constructor
        // -----------
        explicit DrumTabPartDrawerHelper(const QRect& drawingArea, int maxPosition);
        DrumTabPartDrawerHelper(const DrumTabPartDrawerHelper&) = delete;
        DrumTabPartDrawerHelper(DrumTabPartDrawerHelper&&) = delete;
        DrumTabPartDrawerHelper& operator=(const DrumTabPartDrawerHelper&) = delete;
        DrumTabPartDrawerHelper() = delete;

        ~DrumTabPartDrawerHelper() = default;

        // 1. getter using the vector
        // --------------------------
        // return the rectangle of this drumKit at this position in a normal drum tab part
        const QRect& getQRect(Drum::DrumKit drumKit, int position) const;
        // return all possible Drumkit in this position (at least one is returned)
        std::vector<Drum::DrumKit> getDrumKits(int x,int y) const;
        // return the rectangle where drum kit can be placed at this position
        // the returned value can be nullptr meaining that this x-y position does not refer
        // to a rectangle.
        const QRect* getQRect(int x,int y) const;
        // give the drumKitHorizonLine of a x-y position (can return None)
        Drum::DrumKitHorizontalLine getDrumKitHorizontalLine(int x,int y) const;
        // give the position of a given x-y
        int getPosition(int x,int y) const;

        // 2. drawing area getter
        // ----------------------
        const QRect& getDrawingArea() const {return m_drawingAreaQRect;}

        // 3. drum tab part grid
        // ---------------------
        QRect getDrumTabPartHorizontalLine(Drum::DrumKitHorizontalLine drumKitHorizontalLine) const;
        QRect getDrumTabPartVerticalLine(bool leftLineIsAsked = false) const;

        // 4. DrumLine QRect
        // -----------------
        std::vector<QRect> getDrumLine(const std::vector<Drum::DrumLine*>& drumLines) const;


        // recompute method will recreate the entire vector
        void recompute(const QRect& drawingArea, int maxPosition);

        // getter of the geometry
        const DrumTabPartGeometry& getDrumTabPartGeometry() const {return m_drumTabPartGeometry;}

    private:
        std::vector<std::tuple<Drum::DrumKitHorizontalLine,int,QRect>> m_drumKitPositionQRect;
        QRect               m_drawingAreaQRect;
        DrumTabPartGeometry m_drumTabPartGeometry;
        int                 m_maxPosition;

    };
}

#endif // DRUMKITPOSITIONDRAWINGRECTANGLE_H
