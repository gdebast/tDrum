#include "DrumAPI/drum_drumtab.h"
#include "DrumAPI/drum_drumtabfactory.h"
#include "DrumAPI/drum_drumexception.h"

#include "fstream"

using namespace Drum;

DrumTabFactory::DrumTabFactory()
{}

DrumTabFactory::~DrumTabFactory() = default;


DrumTab& DrumTabFactory::loadObject(const std::string& file)
{

    // prepare the reading
    std::ifstream fileStream(file.c_str());

    DrumException::drumAssert(static_cast<bool>(fileStream),
                              "Error from 'DrumTabFactory::loadDrumTab': '{}' cannot be read.",
                              file);

    // returned drumtab
    auto returnedDrumTab = std::make_unique<DrumTab>();

    // read line per line
    std::string currentLine;
    std::string cumulativeLine;
    while (std::getline(fileStream,currentLine))
    {

        cumulativeLine += currentLine;
    }

    returnedDrumTab->fillFromSerialized(cumulativeLine);

    m_AllCreatedDrumTab.push_back(std::make_pair(std::move(returnedDrumTab),file));
    return *(*m_AllCreatedDrumTab.rbegin()).first.get();
}

void DrumTabFactory::dumpToFile() const
{

    for(const auto& tabFilePair : m_AllCreatedDrumTab)
    {
        auto& tab = tabFilePair.first;
        std::string serialized = tab->getSerialized();

        std::ofstream flow(tabFilePair.second);
        if(flow)
        {
            flow << serialized;
            flow.close();
        }
    }
}
