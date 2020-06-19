#include "DrumAPI/drum_drumtabpartfactory.h"

#include "DrumAPI/drum_drumtabpart.h"
#include "DrumAPI/drum_drumexception.h"
#include "Tools/tools_serializerhelper.h"

#include "fstream"


using namespace Drum;

DrumTabPartFactory::DrumTabPartFactory()
{
}

DrumTabPartFactory::~DrumTabPartFactory() = default;


DrumTabPart& DrumTabPartFactory::loadObject(const std::string &file)
{

    // prepare the reading
    std::ifstream fileStrean(file.c_str());
    std::string currentLine;
    if(!fileStrean)
    {
        throw DrumException("Error from 'DrumTabPartFactory::loadObject': '" + file +
                            "' cannot be read.");
    }

    // returned drumtab
    auto returnedDrumTabPart = std::make_unique<DrumTabPart>(file);

    // read line per line
    std::string cumulativeLine;
    while (std::getline(fileStrean,currentLine))
    {

        cumulativeLine += currentLine;
    }

    Tools::SerializerHelper helper(cumulativeLine);
    returnedDrumTabPart->fillFromSerialized(helper);

    m_AllCreatedDrumTabPart.push_back(std::move(returnedDrumTabPart));
    return *(*m_AllCreatedDrumTabPart.rbegin()).get();
}

void DrumTabPartFactory::dumpToFile() const
{
    for(const auto& tabPart : m_AllCreatedDrumTabPart)
    {
        std::string serialized = tabPart->getSerialized();

        std::ofstream flow(tabPart->getSaveFileLocation().c_str());
        if(flow)
        {
            flow << serialized;
            flow.close();
        }
    }
}


