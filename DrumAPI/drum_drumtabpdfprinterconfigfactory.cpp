#include "DrumAPI/drum_drumtabpdfprinterconfigfactory.h"

#include "DrumAPI/drum_drumtabpdfprinterconfig.h"
#include "DrumAPI/drum_drumexception.h"

#include "fstream"

using namespace Drum;

DrumTabPdfPrinterConfigFactory::DrumTabPdfPrinterConfigFactory()
{

}

DrumTabPdfPrinterConfig &DrumTabPdfPrinterConfigFactory::loadObject(const std::string &file)
{
    // prepare the reading
    std::ifstream fileStream(file.c_str());
    DrumException::drumAssert(static_cast<bool>(fileStream),
                              "Error from 'DrumTabPdfPrinterConfigFactory::loadDrumTab': '{}' cannot be read.",
                              file);

    // returned drumtab
    auto createdDrumTabPdfPrinterConfig = std::make_unique<DrumTabPdfPrinterConfig>();

    // read line per line
    std::string cumulativeLine;
    std::string currentLine;
    while (std::getline(fileStream,currentLine))
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
