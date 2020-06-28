#include "DrumAPI/drum_drumtabpartfactory.h"

#include "Tools/tools_serializerhelper.h"

using namespace Drum;

DrumTabPartFactory::DrumTabPartFactory(const Tools::Directory &workingDirectory) :
    IDrumFactory<DrumTabPart>("drumpart",workingDirectory) {}

DrumTabPartFactory::~DrumTabPartFactory() = default;


DrumTabPart &DrumTabPartFactory::createDefaultObject()
{

    auto returnedObject = std::make_unique<DrumTabPart>();
    returnedObject->setDrumTime(16);

    m_AllCreatedObjects.push_back(std::make_pair(std::move(returnedObject),getDefaultFile()));
    return *(*m_AllCreatedObjects.rbegin()).first.get();

}
