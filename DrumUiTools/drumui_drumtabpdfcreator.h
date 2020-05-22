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

        void drawIcon(const QIcon& icon, const QRect& drawingArea, QPainter& painter) const;

        const Drum::DrumTab&                 m_drumTab;
        const Drum::DrumTabPdfPrinterConfig& m_drumTabPdfPrinterConfig;
    };

}

