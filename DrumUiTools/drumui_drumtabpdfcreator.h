#pragma once

#include <QIcon>
#include <QRect>
#include <string>

namespace Drum
{
    class DrumTab;
    class DrumTabPdfPrinterConfig;
}

namespace DrumUI
{

    class DrumTabPdfCreator
    {
    /*
     * class creating pdf files with a DrumTab and a output file as argument.
     */

    public:
        explicit DrumTabPdfCreator(const Drum::DrumTab& drumTab,
                                   const Drum::DrumTabPdfPrinterConfig& pdfConfig);
        DrumTabPdfCreator(const DrumTabPdfCreator&) = delete;
        DrumTabPdfCreator(DrumTabPdfCreator&&) = delete;
        DrumTabPdfCreator& operator=(const DrumTabPdfCreator&) = delete;
        DrumTabPdfCreator() = delete;
        ~DrumTabPdfCreator() = default;

        void createPdf();
    private:

        /**
         * @brief draw an icon given as first argument in the area and using the painter.
         * @param icon, icon to draw
         * @param drawingArea, rectangle where to draw the Icon
         * @param painter, painter used to draw the icon.
         */
        void drawIcon(const QIcon& icon, const QRect& drawingArea, QPainter& painter) const;

        /**
         * @brief draw a rectangle which indicates an implicit repetition.
         *        The rectangle indicates that a number of identical parts
         *        of the drum tab should be repeated and the part size.
         * @param drawingArea, rectangle where to draw
         * @param identicalPartNumber, number of time the part is repeated
         * @param repetitionSize, size of the repeated part
         */
        void drawRepetition(const QRect& drawingArea, unsigned identicalPartNumber, unsigned partSize);

        const Drum::DrumTab&                 m_drumTab;
        const Drum::DrumTabPdfPrinterConfig& m_drumTabPdfPrinterConfig;
    };

}

