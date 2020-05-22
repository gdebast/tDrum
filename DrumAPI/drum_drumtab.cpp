#include "DrumAPI/drum_drumtab.h"
#include "DrumAPI/drum_drumtabpart.h"
#include "DrumAPI/drum_drumexception.h"
#include "Tools/tools_serializerhelper_ext.h"

#include "algorithm"

namespace
{
    static const std::string TITLE        = "title";
    static const std::string AUTHOR       = "author";
    static const std::string DRUMTABPARTS = "DrumTabParts";
    static const std::string PART         = "Part";
    static const std::string ISIMPLICIT   = "IsImplicit";
}


using namespace Drum;

// ============================
// == Constructor/Destructor ==
// ============================

DrumTab::DrumTab() :
      m_author(""),
      m_title("")
{

}

DrumTab::~DrumTab()
{
    for(const auto& pair : m_drumTabParts)
    {
        delete pair.second.first;
    }
}

// ======================
// == Public Functions ==
// ======================

DrumTabPart* DrumTab::addDrumTabPart(unsigned index)
{
    DrumTabPart* newPart(nullptr);

    // if the size is the index, it means the index is not yet taken
    if(index == m_drumTabParts.size())
    {
        newPart = new DrumTabPart();
        m_drumTabParts[index] = std::make_pair(newPart,false);
    }
    // if the index is less than the size,
    // we have to shift all the part after this index
    else if(index < m_drumTabParts.size())
    {
        decltype(m_drumTabParts) newdrumTabParts;
        for(const auto& positionPartPair : m_drumTabParts)
        {
            unsigned oldIndex = positionPartPair.first;
            if(oldIndex >= index)
            {
                newdrumTabParts[oldIndex+1] = positionPartPair.second ;
            }
            else
            {
                newdrumTabParts[oldIndex] = positionPartPair.second;
            }

        }
        newPart = new DrumTabPart();
        newdrumTabParts[index] = std::make_pair(newPart,false);
        m_drumTabParts = newdrumTabParts;
    }
    // otherwise, it is impossible and means that the caller does not know where to put it.
    else
    {
        Tools::StringFunction str;
        auto message = str.CFormatter("Error from 'DrumTab::addDrumTabPart': trying to add at index {} "
                                      "which is more than the size.",
                                      index);
        throw DrumException(message);
    }

    return newPart;
}

void DrumTab::removeDrumTabPart(unsigned index)
{
    if(index >= m_drumTabParts.size())
    {
        Tools::StringFunction str;
        auto message = str.CFormatter("Error from 'DrumTab::removeDrumTabPart': cannot remove index {} "
                                      "when size is {}.",
                                      index,std::to_string(m_drumTabParts.size()));
        throw DrumException(message);
    }

    if(m_drumTabParts.find(index) == m_drumTabParts.end())
    {
        Tools::StringFunction str;
        auto message = str.CFormatter("Error from 'DrumTab::removeDrumTabPart': cannot remove index {} "
                                      "because it does not exist.",
                                      index);
        throw DrumException(message);
    }

    delete m_drumTabParts[index].first;

    m_drumTabParts.erase(index);

    // shift all indexes
    decltype(m_drumTabParts) newDrumTabParts;
    for(const auto& pair : m_drumTabParts)
    {
        if(pair.first > index)
        {
            newDrumTabParts[pair.first-1] = pair.second;

        }
        else
        {
            newDrumTabParts[pair.first] = pair.second;
        }
    }
    m_drumTabParts = newDrumTabParts;


}

void DrumTab::setImplicitExplicit(const DrumTabPart *part, bool implicitExplicit)
{
    for(const auto& positionPartBoolPair : m_drumTabParts)
    {
        if (positionPartBoolPair.second.first == part)
        {
            m_drumTabParts[positionPartBoolPair.first] = std::make_pair(positionPartBoolPair.second.first,implicitExplicit);
            break;
        }
    }
}

unsigned DrumTab::getDrumTabSize() const
{
    return m_drumTabParts.size();
}

std::vector<std::pair<DrumTabPart*,bool>> DrumTab::getDrumTabParts() const
{
    std::vector<std::pair<DrumTabPart*,bool>> returnedparts;
    for(const auto& pair : m_drumTabParts)
    {
        returnedparts.push_back(pair.second);
    }

    return returnedparts;
}

std::string DrumTab::getSerialized() const
{

    // add the settings
    Tools::SerializerHelper helper;
    helper.addToSerializeString(TITLE,m_title);
    helper.addToSerializeString(AUTHOR,m_author);

    // sub helper for the drum tab parts
    auto& drumTabPartsHelper = helper.addSubSerializerHelper(DRUMTABPARTS);

    unsigned index(0);
    unsigned size(m_drumTabParts.size());

    while (index < size)
    {
        auto it = m_drumTabParts.find(index);

        // assert if we cannot find the index
        if(it == m_drumTabParts.end())
        {
            Tools::StringFunction str;
            auto message = str.CFormatter("Error from 'DrumTab::getSerialized' : index {} "
                                          "is not contained in the tab.",
                                          index);
            throw DrumException(message);
        }

        auto& currentDrumTabPartLinkHelper = drumTabPartsHelper.addSubSerializerHelper(std::to_string(index));
        auto& currentDrumTabPartHelper = currentDrumTabPartLinkHelper.addSubSerializerHelper(PART);
        it->second.first->fillSerializer(currentDrumTabPartHelper);

        // add the fact that it is explicit or not
        currentDrumTabPartLinkHelper.addToSerializeString(ISIMPLICIT,it->second.second);

        index++;
    }

    return helper.getSerializedString();
}

void DrumTab::fillFromSerialized(const std::string &serializedString)
{
    Tools::SerializerHelper helper(serializedString);
    helper.deserialize(TITLE,m_title);
    helper.deserialize(AUTHOR,m_author);

    if (!helper.isSerializerHelper(DRUMTABPARTS))
        return;


    auto& drumTabPartsHelper = helper.getSubSerializerHelper(DRUMTABPARTS);

    unsigned indexDrumTabPart(0);
    while(true)
    {
        // stop if there are no more helper
        auto& currentDrumTabPartLinkHelper = drumTabPartsHelper.getSubSerializerHelper(std::to_string(indexDrumTabPart));
        if (&currentDrumTabPartLinkHelper == &drumTabPartsHelper)
            break;

        // create a new drum tab part by finding the sub helper with the part
        auto newPart = new DrumTabPart();
        newPart->fillFromSerialized(currentDrumTabPartLinkHelper.getSubSerializerHelper(PART));

        // get if it is implicit or not
        bool isImplicit(false);
        currentDrumTabPartLinkHelper.deserialize(ISIMPLICIT,isImplicit);

        m_drumTabParts[m_drumTabParts.size()] = std::make_pair(newPart,isImplicit);

        indexDrumTabPart++;
    }


}
