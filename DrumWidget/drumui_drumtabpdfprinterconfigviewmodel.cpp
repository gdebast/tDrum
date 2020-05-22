#include "DrumWidget/drumui_drumtabpdfprinterconfigviewmodel.h"

#include <DrumAPI/drum_drumtabpdfprinterconfig.h>

using namespace DrumUI;

DrumTabPdfPrinterConfigViewModel::DrumTabPdfPrinterConfigViewModel(Drum::DrumTabPdfPrinterConfig &model) :
    m_model(model),
    m_explicitDrumTabPrinting(model.isDrumTabPrintingExplicit()),
    m_pdfExportDirectory(model.getPdfExportDirectory().c_str())
{
}

void DrumTabPdfPrinterConfigViewModel::synchronizeModel() const
{
    m_model.setExplicitDrumTabPrinting(m_explicitDrumTabPrinting);
    m_model.setPdfExportDirector(m_pdfExportDirectory.toStdString());
}
