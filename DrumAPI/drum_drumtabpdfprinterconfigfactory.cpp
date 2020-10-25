#include "DrumAPI/drum_drumtabpdfprinterconfigfactory.h"

#include "Tools/tools_exception.h"


using namespace Drum;

DrumTabPdfPrinterConfigFactory::DrumTabPdfPrinterConfigFactory(const Tools::Directory &workingDirectory) :
    IDrumFactory<DrumTabPdfPrinterConfig>("pdfconfig",workingDirectory) {}


DrumTabPdfPrinterConfig &DrumTabPdfPrinterConfigFactory::createDefaultObject()
{

    auto returnedObject = std::make_unique<DrumTabPdfPrinterConfig>();
    returnedObject->setPdfExportDirector(m_workingDirectory.getDirectoryPath());

    m_AllCreatedObjects.push_back(std::make_pair(std::move(returnedObject),getDefaultFile()));
    return *(*m_AllCreatedObjects.rbegin()).first.get();

}

DrumTabPdfPrinterConfigFactory::~DrumTabPdfPrinterConfigFactory() = default;
