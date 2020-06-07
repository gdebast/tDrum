#include "DrumUiTools/drumui_pdfpagemanager.h"

#include "DrumUiTools/drumui_drumtabpartgeometry.h"

#include <QIcon>
#include <QPainter>
#include <QRect>


DrumUI::PdfPageManager::PdfPageManager(QPdfWriter &pdf,
                                       QPainter &painter,
                                       unsigned numberOfDrumTabPartPerRow,
                                       double leftRightMargin_Column_Ratio,
                                       double upperLowerMargin_Column_Ratio) :
    m_pdfWriter(pdf),
    m_painter(painter),
    m_numberOfDrumTabPartPerRow(numberOfDrumTabPartPerRow),
    m_leftRightMargin_Column_Ratio(leftRightMargin_Column_Ratio),
    m_upperLowerMargin_Column_Ratio(upperLowerMargin_Column_Ratio)
{
    // calculated sizes
    m_pdfYSize = pdf.height();
    m_pdfXSize = pdf.width();
    m_columnSize = static_cast<double>(m_pdfXSize)/
                                static_cast<double>(static_cast<double>(m_numberOfDrumTabPartPerRow)+2.0*m_leftRightMargin_Column_Ratio);
    m_rowSize = static_cast<double>(DrumTabPartGeometry::getBaseHeight())/static_cast<double>(DrumTabPartGeometry::getBaseWidght())*
                             m_columnSize;
    m_leftRightMarginSize = static_cast<double>(m_columnSize)*m_leftRightMargin_Column_Ratio;
    m_upperBottomMarginSize = static_cast<double>(m_rowSize)*m_upperLowerMargin_Column_Ratio;

    // rectangle where to put the icon of tDrum (top-left corner of the drum sheet)
    const unsigned tDrumSquareWidth(std::max(m_leftRightMarginSize,m_upperBottomMarginSize));
    const QRect tDrumSquare(0,0,tDrumSquareWidth,tDrumSquareWidth);
    const double MARGIN_SQUAREWIDTH_RATIO(0.1);
    const unsigned tDrumIconMargin = MARGIN_SQUAREWIDTH_RATIO*static_cast<double>(tDrumSquare.width());
    const unsigned tDrumIconWidth = tDrumSquare.width() - 2*tDrumIconMargin;
    m_tDrumIconSquare = QRect(tDrumIconMargin+tDrumSquare.x(),tDrumIconMargin+tDrumSquare.y(),
                              tDrumIconWidth,tDrumIconWidth);

    // start by creating the tDrum icon
    drawtDrumIcon();
}

QRect DrumUI::PdfPageManager::getDrawingArea()
{

    if (m_createNewPage)
    {
        m_pdfWriter.newPage();
        drawtDrumIcon();
        m_createNewPage = false;
    }

    unsigned columNr = m_index_drumTabPart%m_numberOfDrumTabPartPerRow;

    unsigned xCorner = m_leftRightMarginSize + columNr*m_columnSize;
    unsigned yCorner = m_upperBottomMarginSize + m_rowNr*m_rowSize;

    //drawing Area
    QRect drawingArea(xCorner,yCorner,m_columnSize,m_rowSize);

    m_index_drumTabPart++;

    // compute the row. We only change row if the next column number is zero
    if (m_index_drumTabPart%m_numberOfDrumTabPartPerRow == 0)
    {
        m_rowNr++;
        // we create a new page if the current number of row (rowNr+1)
        // makes the total size larger than the pdf height.
        if (2*m_upperBottomMarginSize + (m_rowNr+1)*m_rowSize > m_pdfYSize)
        {
            m_rowNr = 0;
            // send a trigger to create a new page such that only
            // a new row creates it
            m_createNewPage = true;
        }
    }

    return drawingArea;

}

void DrumUI::PdfPageManager::drawtDrumIcon() const
{
    int pixel(2000);

    QIcon icon(":/tDrum_GreenTea.svg");

    m_painter.drawPixmap(m_tDrumIconSquare,
                        icon.pixmap(pixel,pixel),
                        QRect(0,0,pixel,pixel));
}
