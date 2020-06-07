#pragma once

#include <QPdfWriter>

namespace DrumUI {


    class PdfPageManager
    {
        public:
            PdfPageManager(QPdfWriter& pdf,
                           QPainter &painter,
                           unsigned numberOfDrumTabPartPerRow,
                           double leftRightMargin_Column_Ratio,
                           double upperLowerMargin_Column_Ratio);
            PdfPageManager(const PdfPageManager&) = delete;
            PdfPageManager(PdfPageManager&&) = delete;
            PdfPageManager& operator=(const PdfPageManager&) = delete;
            PdfPageManager& operator=(PdfPageManager&&) = delete;
            ~PdfPageManager() = default;

            /**
             * \brief return the QRect where to draw currently
             * \return QRect where to draw
             */
            QRect getDrawingArea();


        private:
            // given
            QPdfWriter&    m_pdfWriter;
            QPainter&      m_painter;
            const unsigned m_numberOfDrumTabPartPerRow;
            const double   m_leftRightMargin_Column_Ratio;
            const double   m_upperLowerMargin_Column_Ratio;

            // computed once
            unsigned m_pdfYSize;
            unsigned m_pdfXSize;
            unsigned m_columnSize;
            unsigned m_rowSize;
            unsigned m_leftRightMarginSize;
            unsigned m_upperBottomMarginSize;
            QRect    m_tDrumIconSquare;

            // incrementally changed
            unsigned m_index_drumTabPart{0};
            unsigned m_rowNr{0};
            bool     m_createNewPage{false};


            /**
             * @brief draw an icon given as first argument in the area and using the painter.
             */
            void drawtDrumIcon() const;

    };

}


