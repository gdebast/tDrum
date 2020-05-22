#include "DrumAPI/drum_drumtab.h"
#include "DrumAPI/drum_drumtabfactory.h"
#include "DrumAPI/drum_drumexception.h"

#include "fstream"

using namespace Drum;

DrumTabFactory::DrumTabFactory()
{}

DrumTabFactory::~DrumTabFactory() = default;


DrumTab* DrumTabFactory::loadObject(const std::string& file)
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
    auto returnedDrumTab = std::make_unique<DrumTab>();

    // read line per line
    std::string cumulativeLine;
    while (std::getline(fileStrean,currentLine))
    {

        cumulativeLine += currentLine;
    }

    returnedDrumTab->fillFromSerialized(cumulativeLine);

    m_AllCreatedDrumTab.push_back(std::make_pair(std::move(returnedDrumTab),file));
    return (*m_AllCreatedDrumTab.rbegin()).first.get();
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
