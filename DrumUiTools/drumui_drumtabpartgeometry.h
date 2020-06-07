#ifndef DRUMTABPARTGEOMETRY_H
#define DRUMTABPARTGEOMETRY_H

namespace DrumUI {

    class DrumTabPartGeometry
    {
    /*
     * This class contains the typical sizes and positions of a DrumTabPart.
     * Those size and position are relative to the upper-left corner of the
     * DrumTabPart drawing area.
     */


    public:
        explicit DrumTabPartGeometry(int width, int height);
        DrumTabPartGeometry(const DrumTabPartGeometry&) = delete;
        DrumTabPartGeometry(DrumTabPartGeometry&&) = delete;
        DrumTabPartGeometry& operator=(const DrumTabPartGeometry&) = delete;
        DrumTabPartGeometry() = delete;

        static int getBaseHeight();
        static int getBaseWidght();

        // recompute all members
        void recompute(int width, int height);

        //getters
        int getHeight() const;
        int getWidth() const;
        int getHiHatFloorLine() const;
        int getBassDrumLine() const;
        int getFloorTomLine() const;
        int getSnareTomLine() const;
        int getHighTomLine() const;
        int getHiHatLine() const;
        int getCrashLine() const;
        int getHorizontalLineThickness() const;
        int getVerticalLineThickness() const;
        int getHorizontalCrocheLineThickness() const;
        int getLineSpacing() const;
        int getDrumKitXsize() const;
        int getVerticalLineUpperEnd() const;
        int getVertivalLineBottomEnd() const;
        int getCymbalXThickness() const;
        int getCymbalYThickness() const;
        int getRepetitionMarkerWidth() const;

    private:
        // constant
        static const int HLINETHICHNESSRATIO{100};
        static const int VLINETHICHNESSRATIO{10};
        static const int VOIDSRATIO{4};
        static const int VOIDSENDLINERATIO{3};
        static const int LINENUMBER{6};
        static const int BASEHEIGHT{120};
        static const int BASEWIDTH{220};
        static const int WIDTHRXRATIO{20};
        static const int DRUMKITSIZE_REPETITIONMARKER_RATIO{2}; // ratio between the size of a drumkit and the size of a repetition marker

        // member
        int m_width;
        int m_height;

        // calculated:
        // thickness of the rectangle making a Cymbal
        int m_cymbalXThickness, m_cymbalYThickness;
        //  lines y coordinates
        int m_hihatFloorLine, m_bassDrumLine, m_floorTomLine, m_snareTomLine, m_highTomLine, m_hiHatLine, m_crashLine;
        //  line spacing
        int m_lineSpacing;
        //  line thickness
        int m_verticalLineThickness;
        int m_horizontalLineThickness;
        int m_horizontalCrocheLineThickness;
        //  drum kit size
        int m_drumKitXsize;
        //  end of the drum kit vertical lines
        int m_verticalLineUpperEnd;
        int m_vertivalLineBottomEnd;
        // repetition triangle width
        int m_repetitionMarkerWidth;

    };

}

#endif // DRUMTABPARTGEOMETRY_H
