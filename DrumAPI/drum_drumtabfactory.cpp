#include "DrumAPI/drum_drumtabfactory.h"

#include "Tools/tools_exception.h"

#include "fstream"

using namespace Drum;

DrumTabFactory::DrumTabFactory(const Tools::Directory &workingDirectory) :
    IDrumFactory<DrumTab>("drum", workingDirectory) {}

DrumTabFactory::~DrumTabFactory() = default;


DrumTab &DrumTabFactory::createDefaultObject()
{
    auto returnedObject = std::make_unique<DrumTab>();
    for (unsigned i(0); i < 4; i++)
    {
        returnedObject->addDrumTabPart(i);
    }

    m_AllCreatedObjects.push_back(std::make_pair(std::move(returnedObject),getDefaultFile()));
    return *(*m_AllCreatedObjects.rbegin()).first.get();
}
