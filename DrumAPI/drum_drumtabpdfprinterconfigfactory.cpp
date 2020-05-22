#include "DrumAPI/drum_drumtabpdfprinterconfigfactory.h"

#include "DrumAPI/drum_drumtabpdfprinterconfig.h"
#include "DrumAPI/drum_drumexception.h"

#include "fstream"

using namespace Drum;

DrumTabPdfPrinterConfigFactory::DrumTabPdfPrinterConfigFactory()
{

}

DrumTabPdfPrinterConfig &DrumTabPdfPrinterConfigFactory::loadFromFile(const std::string &file)
{
    // prepare the reading
    std::ifstream fileStrean(file.c_str());
    std::string currentLine;
    if(!fileStrean)
    {
        throw DrumException("Error from 'DrumTabFactory::loadDrumTab': '" + file +
                            "' cannot be read.");
    }

    // returned drumtab
    auto createdDrumTabPdfPrinterConfig = std::make_unique<DrumTabPdfPrinterConfig>();

    // read line per line
    std::string cumulativeLine;
    while (std::getline(fileStrean,currentLine))
    {
        cumulativeLine += currentLine;
    }
    createdDrumTabPdfPrinterConfig->fillFromSerialized(cumulativeLine);

    m_allCreatedObjects.push_back(std::make_pair(move(createdDrumTabPdfPrinterConfig),file));
    auto & result = *(*m_allCreatedObjects.rbegin()).first.get();
    return result;
}

void DrumTabPdfPrinterConfigFactory::dumpToFile() const
{
    for(const auto& object : m_allCreatedObjects)
    {
        std::string serialized = object.first->getSerialized();
        std::ofstream flow(object.second.c_str());

        if(flow)
        {
            flow << serialized;
            flow.close();
        }
    }
}

DrumTabPdfPrinterConfigFactory::~DrumTabPdfPrinterConfigFactory() = default;
