#include "DrumUiTools/drumui_drumtabpdfcreator.h"

#include "DrumAPI/drum_drumtab.h"
#include "DrumAPI/drum_drumtabpart.h"
#include "DrumAPI/drum_drumtabimpliciter.h"
#include "DrumUiTools/drumui_drumtabpartgeometry.h"
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
    std::string fullPath =   m_drumTabPdfPrinterConfig.getPdfExportDirectory() + "tDrum_"
                           + m_drumTab.getAuthor() + "_"
                           + m_drumTab.getTitle()  + ".pdf";
    // trimming
    fullPath.erase(std::remove(fullPath.begin(),
                               fullPath.end(),
                                            ' '),
                               fullPath.end());
    const QString filename(fullPath.c_str());



    QPdfWriter pdfwriter(filename);
    pdfwriter.setPageSize(QPageSize(QPageSize::A4));
    QPainter painter(&pdfwriter);

    // constant for the drawing
    const unsigned NUMBEROFCOLUMN(4);
    const double   LEFTRIGHTMARGIN_COLUMN_RATIO(0.5); //left and right margin/column size
    const double   UPPPERBOTTOMMARGIN_ROW_RATIO(1.0); // upper and bottom margin/row size
    // calculated sizes
    const unsigned pdfYSize(pdfwriter.height());
    const unsigned pdfXSize(pdfwriter.width());
    const unsigned columnSize = static_cast<double>(pdfXSize)/
                                static_cast<double>(static_cast<double>(NUMBEROFCOLUMN)+2.0*LEFTRIGHTMARGIN_COLUMN_RATIO);
    const unsigned rowSize = static_cast<double>(DrumTabPartGeometry::getBaseHeight())/static_cast<double>(DrumTabPartGeometry::getBaseWidght())*
                             columnSize;
    const unsigned leftRightMarginSize = static_cast<double>(columnSize)*LEFTRIGHTMARGIN_COLUMN_RATIO;
    const unsigned upperBottomMarginSize = static_cast<double>(rowSize)*UPPPERBOTTOMMARGIN_ROW_RATIO;

    // rectangle where to put the icon of tDrum (top-left corner of the drum sheet)
    const unsigned tDrumSquareWidth(std::max(leftRightMarginSize,upperBottomMarginSize));
    const QRect tDrumSquare(0,0,tDrumSquareWidth,tDrumSquareWidth);
    const double MARGIN_SQUAREWIDTH_RATIO(0.1);
    const unsigned tDrumIconMargin = MARGIN_SQUAREWIDTH_RATIO*static_cast<double>(tDrumSquare.width());
    const unsigned tDrumIconWidth = tDrumSquare.width() - 2*tDrumIconMargin;
    const QRect tDrumIconSquare(tDrumIconMargin+tDrumSquare.x(),tDrumIconMargin+tDrumSquare.y(),
                                tDrumIconWidth,tDrumIconWidth);

    // paint the icon
    drawIcon(QIcon(":/tDrum_GreenTea.svg"),tDrumIconSquare,painter);

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
    unsigned index_drumTabPart(0);
    unsigned rowNr(0);
    bool createNewPage(false);
    for (const auto& drumTabPartImplicitPair : m_drumTab.getDrumTabParts())
    {
        auto* drumTabPart = drumTabPartImplicitPair.first;

        // trigger to create a new page
        if (createNewPage)
        {
            pdfwriter.newPage();
            drawIcon(QIcon(":/tDrum_GreenTea.svg"),tDrumIconSquare, painter);
            createNewPage = false;
        }


        unsigned columNr = index_drumTabPart%NUMBEROFCOLUMN;

        unsigned xCorner = leftRightMarginSize + columNr*columnSize;
        unsigned yCorner = upperBottomMarginSize + rowNr*rowSize;

        //drawing Area
        QRect drawingArea(xCorner,yCorner,columnSize,rowSize);
        //drawing helper
        DrumTabPartDrawerHelper helper(drawingArea,drumTabPart->getDrumTime());
        DrumTabPartDrawer drawer(pdfwriter,*drumTabPart);
        drawer.DrawDrumTabPart(helper,painter);

        index_drumTabPart++;

        // compute the row. We only change row if the next column number is zero
        if (index_drumTabPart%NUMBEROFCOLUMN == 0)
        {
            rowNr++;
            // we create a new page if the current number of row (rowNr+1)
            // makes the total size larger than the pdf height.
            if (2*upperBottomMarginSize + (rowNr+1)*rowSize > pdfYSize)
            {
                rowNr = 0;
                // send a trigger to create a new page such that only
                // a new row creates it
                createNewPage = true;
            }
        }

    }


}

void DrumTabPdfCreator::drawIcon(const QIcon &icon,
                                 const QRect &drawingArea,
                                 QPainter& painter) const
{
    int pixel(2000);

    painter.drawPixmap(drawingArea,
                       icon.pixmap(pixel,pixel),
                       QRect(0,0,pixel,pixel));
}
