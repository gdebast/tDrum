#include "DrumUiTools/drumui_drumtabpartgeometry.h"

using namespace DrumUI;


DrumTabPartGeometry::DrumTabPartGeometry(int width, int height)
{
    recompute(width, height);
}


// ======================
// == public functions ==
// ======================

// Getters
// -------


int DrumUI::DrumTabPartGeometry::getBaseHeight()
{
    return BASEHEIGHT;
}

int DrumTabPartGeometry::getBaseWidght()
{
    return BASEWIDTH;
}

void DrumTabPartGeometry::recompute(int width, int height)
{
    m_width = width;
    m_height = height;

    /*
     *  map of the lines:
     *
     *                   Crash
     *  ---------------- HiHat
     *  ---------------- HighTom
     *  ---------------- SnareTom
     *  ---------------- FloorTom
     *  ---------------- BassDrum
     *
     */

    // thichness of the vertical and horizontal lines
    m_horizontalLineThickness = m_height/HLINETHICHNESSRATIO;
    m_verticalLineThickness = m_horizontalLineThickness/VLINETHICHNESSRATIO;
    m_horizontalCrocheLineThickness = m_horizontalLineThickness*3;
    // Cymbal thickness : X and Y thicknesses of the two rectangle that make the cross
    //      / ^
    //     /  | y thickness
    //    /   v
    //    <--->
    //     x thickness
    m_cymbalYThickness = m_verticalLineThickness;
    m_cymbalXThickness = m_horizontalLineThickness;

    // space between the horizontal lines
    m_lineSpacing = (height - LINENUMBER*m_horizontalLineThickness - 2*height/VOIDSRATIO)/(LINENUMBER-1);

    // line y coordinates
    m_crashLine = height/VOIDSRATIO;
    m_hiHatLine = m_crashLine + m_horizontalLineThickness + m_lineSpacing;
    m_highTomLine = m_hiHatLine + m_horizontalLineThickness + m_lineSpacing;
    m_snareTomLine = m_highTomLine + m_horizontalLineThickness + m_lineSpacing;
    m_floorTomLine = m_snareTomLine + m_horizontalLineThickness + m_lineSpacing;
    m_bassDrumLine = m_floorTomLine + m_horizontalLineThickness + m_lineSpacing;
    m_hihatFloorLine = m_bassDrumLine + m_horizontalLineThickness + m_lineSpacing;

    // geometry parameter for drum kits
    m_drumKitXsize = m_width/WIDTHRXRATIO;

    // vertical line ends
    m_verticalLineUpperEnd  = m_height/VOIDSRATIO/VOIDSENDLINERATIO;
    m_vertivalLineBottomEnd = m_height - m_verticalLineUpperEnd;
}

int DrumTabPartGeometry::getHeight() const
{
    return m_height;
}

int DrumTabPartGeometry::getWidth() const
{
    return m_width;
}

int DrumTabPartGeometry::getHiHatFloorLine() const
{
    return m_hihatFloorLine;
}

int DrumTabPartGeometry::getBassDrumLine() const
{
    return m_bassDrumLine;
}

int DrumTabPartGeometry::getFloorTomLine() const
{
    return m_floorTomLine;
}

int DrumTabPartGeometry::getSnareTomLine() const
{
    return m_snareTomLine;
}

int DrumTabPartGeometry::getHighTomLine() const
{
    return m_highTomLine;
}

int DrumTabPartGeometry::getHiHatLine() const
{
    return m_hiHatLine;
}

int DrumTabPartGeometry::getCrashLine() const
{
    return m_crashLine;
}

int DrumTabPartGeometry::getHorizontalLineThickness() const
{
    return m_horizontalLineThickness;
}

int DrumTabPartGeometry::getVerticalLineThickness() const
{
    return m_verticalLineThickness;
}

int DrumTabPartGeometry::getHorizontalCrocheLineThickness() const
{
    return m_horizontalCrocheLineThickness;
}

int DrumTabPartGeometry::getLineSpacing() const
{
    return m_lineSpacing;
}

int DrumTabPartGeometry::getDrumKitXsize() const
{
    return m_drumKitXsize;
}

int DrumTabPartGeometry::getVerticalLineUpperEnd() const
{
    return m_verticalLineUpperEnd;
}

int DrumTabPartGeometry::getVertivalLineBottomEnd() const
{
    return m_vertivalLineBottomEnd;
}

int DrumTabPartGeometry::getCymbalYThickness() const
{
    return m_cymbalYThickness;
}

int DrumTabPartGeometry::getCymbalXThickness() const
{
    return m_cymbalXThickness;
}
