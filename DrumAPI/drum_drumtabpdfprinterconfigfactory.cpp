#include "DrumAPI/drum_drumtabpdfprinterconfigfactory.h"

#include "DrumAPI/drum_drumexception.h"


using namespace Drum;

DrumTabPdfPrinterConfigFactory::DrumTabPdfPrinterConfigFactory(const Tools::Directory &workingDirectory) :
    IDrumFactory<DrumTabPdfPrinterConfig>("pdfconfig",workingDirectory) {}


DrumTabPdfPrinterConfig &DrumTabPdfPrinterConfigFactory::getOneObject()
{
    // if one exists; return it
    if (m_AllCreatedObjects.size() > 0)
    {
        return *m_AllCreatedObjects[0].first.get();
    }

    // otherwise, create an empty drum tab
    auto returnedObject = std::make_unique<DrumTabPdfPrinterConfig>();


    m_AllCreatedObjects.push_back(std::make_pair(std::move(returnedObject),getDefaultFile()));
    return *(*m_AllCreatedObjects.rbegin()).first.get();

}

DrumTabPdfPrinterConfigFactory::~DrumTabPdfPrinterConfigFactory() = default;
