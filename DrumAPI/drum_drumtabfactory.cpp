#include "DrumAPI/drum_drumtabfactory.h"

#include "DrumAPI/drum_drumexception.h"

#include "fstream"

using namespace Drum;

DrumTabFactory::DrumTabFactory(const Tools::Directory &workingDirectory) :
    IDrumFactory<DrumTab>("drum", workingDirectory) {}

DrumTabFactory::~DrumTabFactory() = default;


DrumTab &DrumTabFactory::getOneObject()
{
    // if one exists; return it
    if (m_AllCreatedObjects.size() > 0)
    {
        return *m_AllCreatedObjects[0].first.get();
    }

    // otherwise, create an empty drum tab
    auto returnedObject = std::make_unique<DrumTab>();
    for (unsigned i(0); i < 4; i++)
    {
        returnedObject->addDrumTabPart(i);
    }

    m_AllCreatedObjects.push_back(std::make_pair(std::move(returnedObject),getDefaultFile()));
    return *(*m_AllCreatedObjects.rbegin()).first.get();

}
