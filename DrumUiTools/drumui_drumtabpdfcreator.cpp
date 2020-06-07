#include "DrumUiTools/drumui_drumtabpdfcreator.h"

#include "DrumAPI/drum_drumtab.h"
#include "DrumAPI/drum_drumtabpart.h"
#include "DrumAPI/drum_drumtabimpliciter.h"
#include "DrumUiTools/drumui_pdfpagemanager.h"
#include "DrumUiTools/drumui_drumtabpartdrawerhelper.h"
#include "DrumUiTools/drumui_drumtabpartdrawer.h"

#include <QPdfWriter>
#include <QString>

#include <DrumAPI/drum_drumtabpdfprinterconfig.h>

using namespace DrumUI;

DrumTabPdfCreator::DrumTabPdfCreator(const Drum::DrumTab& drumTab,
                                     const Drum::DrumTabPdfPrinterConfig& pdfConfig) :
    m_drumTab(drumTab),
    m_drumTabPdfPrinterConfig(pdfConfig)
{

}

void DrumTabPdfCreator::createPdf()
{
    // create the pdf name
    std::string fileName =  "tDrum_"+ m_drumTab.getAuthor() + "_" + m_drumTab.getTitle()  + ".pdf";
    fileName.erase(std::remove(fileName.begin(),
                               fileName.end(),
                               ' '),
                               fileName.end());
    std::string fullPath =   m_drumTabPdfPrinterConfig.getPdfExportDirectory() + fileName;

    // trimming
    const QString qStringfullPath(fullPath.c_str());

    QPdfWriter pdfwriter(qStringfullPath);
    pdfwriter.setPageSize(QPageSize(QPageSize::A4));
    QPainter painter(&pdfwriter);

    // constant for the drawing
    const double   LEFTRIGHTMARGIN_COLUMN_RATIO(0.5); //left and right margin/column size
    const double   UPPPERBOTTOMMARGIN_ROW_RATIO(1.0); // upper and bottom margin/row size

    // page manager
    PdfPageManager pdfPageManager(pdfwriter,
                                  painter,
                                  m_drumTabPdfPrinterConfig.getNumberOfDrumTabPartsPerRow(),
                                  LEFTRIGHTMARGIN_COLUMN_RATIO,
                                  UPPPERBOTTOMMARGIN_ROW_RATIO);


    // create the impliciter to get an explicit or implicit tab
    Drum::DrumTabImpliciter impliciter;
    Drum::DrumTabImpliciter::DrumTabPartRepetition tabToPrint;
    if (m_drumTabPdfPrinterConfig.isDrumTabPrintingExplicit())
    {
        tabToPrint = impliciter.getExplicit(m_drumTab.getDrumTabParts());
    }
    else
    {
        tabToPrint = impliciter.getImplicit(m_drumTab.getDrumTabParts());
    }


    // loop on the drum tab
    DrumTabPartDrawer drawer(pdfwriter);
    for (const auto& [drumTabParts,repetitionNumber] : tabToPrint)
    {
        unsigned index_drumTabPart(0);
        bool repetitionNeeded = repetitionNumber >= 2;

        for (auto* drumTabPart : drumTabParts)
        {
            index_drumTabPart++;

            //drawing Area
            QRect drawingArea = pdfPageManager.getDrawingArea();
            //drawing helper
            DrumTabPartDrawerHelper helper(drawingArea,drumTabPart->getDrumTime());
            drawer.DrawDrumTabPart(*drumTabPart,helper,painter);

            // mark the main repeated part
            if(index_drumTabPart == 1 && repetitionNeeded)
            {
                drawer.markRepeatedPart(drawingArea,painter,false);
            }
            if(repetitionNeeded && index_drumTabPart == drumTabParts.size())
            {
                drawer.markRepeatedPart(drawingArea,painter,true);
            }

        }

        if(repetitionNeeded == true)
        {
            //drawing Area
            QRect drawingArea = pdfPageManager.getDrawingArea();
            drawer.drawRepetion(drawingArea,painter,repetitionNumber-1);
        }


    }


}
