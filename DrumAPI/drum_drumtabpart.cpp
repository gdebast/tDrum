#include "DrumAPI/drum_drumtabpart.h"

#include "DrumAPI/drum_drumexception.h"
#include "DrumAPI/drum_drumkithorizontallinehelper.h"
#include "Tools/tools_serializerhelper_ext.h"

#include <stdexcept>
#include <algorithm>
#include <cmath>
#include "fstream"


namespace
{
    static const std::string TIME     = "Time";
    static const std::string DRUMKITS = "Drumkits";
    static const std::string KIT      = "Kit";
    static const std::string POSITION = "Position";
}

using namespace Drum;

// ============================
// == Constructor/Destructor ==
// ============================

DrumTabPart::DrumTabPart()
{
}

DrumTabPart::DrumTabPart(const std::string &file):
    m_saveFileLocation(file),
    m_hasSaveFile(true)
{
}

DrumTabPart::DrumTabPart(const DrumTabPart& other)
{
    m_drumKitPositions = other.m_drumKitPositions;
    m_drumTime = other.m_drumTime;
    m_isDrumLinesUptoDate = false; // false such that a call to getDrumLines is triggered
    m_drumLines = {};
    m_saveFileLocation = ""; // a in-memory copy does not have a file
    m_hasSaveFile = false;
}

DrumTabPart::~DrumTabPart()
{
    deleteDrumLines();
}

// ======================
// == Public Functions ==
// ======================

void DrumTabPart::clear()
{
    this->operator=(DrumTabPart());
}

void DrumTabPart::setDrumTime(int drumtime)
{
    m_drumTime = drumtime;
    m_isDrumLinesUptoDate = false;
}

void DrumTabPart::addDrumKit(DrumKit drumKit, unsigned position)
{
    // do nothing in case of silence
    if (DrumKit::Silence == drumKit)
        return;

    // erase the drumkit at the same line
    // -----
    Drum::DrumKitHorizontalLineHelper helper;
    auto lines = helper.getDrumKitHorizontalLines(drumKit);
    if (lines.size() != 1)
        // the number of possible lines should be 1
        // as a non-silence drumkit has one line
    {

        auto drumKitStr = getSerializedDrumKit(drumKit);
        throw DrumException("Error from 'Drum::DrumTabPart::addDrumKit': '" + drumKitStr
                            + "' has one or multiple DrumKitHorizontalLines. "
                            + std::to_string(lines.size()) + " found.");
    }
    clearLineAtPosition(lines[0],position);

    // add the new drum kit
    // ---
    m_drumKitPositions.push_back(std::pair<unsigned,DrumKit>(position,drumKit));
    m_isDrumLinesUptoDate = false;
}

DrumTabPart& DrumTabPart::operator=(const DrumTabPart& other)
{
    m_drumKitPositions = other.m_drumKitPositions;
    m_drumTime = other.m_drumTime;
    m_isDrumLinesUptoDate = false;
    return *this;
}

bool DrumTabPart::operator==(const DrumTabPart &other) const
{
    VecPair_DrumSetPartPosition otherDrumKitPositionCopy(other.m_drumKitPositions);

    for (const auto& thisKitPosition : m_drumKitPositions)
    {
        // for each drumkit-Position pair, find one identical pair
        bool identicalPairFound(false);
        auto itOther = otherDrumKitPositionCopy.cbegin();
        while (itOther != otherDrumKitPositionCopy.cend()) {

            identicalPairFound = itOther->first == thisKitPosition.first
                              && itOther->second == thisKitPosition.second;
            if(identicalPairFound == true)
            {
                break;
            }
            else
            {
                itOther++;
            }
        }

        // the identical pair was not found for this pair so they cannot be identical
        if (identicalPairFound == false)
        {
            return false;
        }
        // otherwise, remove the identical pair such that we also tell 'other'
        // that we have found a pair for this one.
        else
        {
            otherDrumKitPositionCopy.erase(itOther);
        }

    }

    // if after matching each pair, the resul is empty, it means that all matched
    return otherDrumKitPositionCopy.empty();
}

void DrumTabPart::sortDrumKits()
{
    std::sort(m_drumKitPositions.begin(),m_drumKitPositions.end(),[](std::pair<unsigned,DrumKit> first, std::pair<unsigned,DrumKit> second){
        return first.first < second.first;
    });

}

int DrumTabPart::getDrumTime() const
{
    return m_drumTime;
}

std::vector<DrumLine*> DrumTabPart::getDrumLines()
{
    createDrumLines();
    return m_drumLines;
}


std::string DrumTabPart::getSerialized() const
{
    Tools::SerializerHelper helper;
    fillSerializer(helper);
    return helper.getSerializedString();
}

std::string DrumTabPart::getSaveFileLocation() const
{
    if(m_hasSaveFile == false)
    {
        throw DrumException("Error from 'DrumTabPart::getSaveFileLocation': this drum tab part has no file location where to save it");
    }

    return m_saveFileLocation;
}

DrumKit DrumTabPart::getDrumKit(DrumKitHorizontalLine line, unsigned position) const
{
    if (line == DrumKitHorizontalLine::None)
    {
        return DrumKit::Silence;
    }


    DrumKitHorizontalLineHelper helper;

    for(const auto &kitPosition : m_drumKitPositions)
    {
        if(kitPosition.first == position
          && kitPosition.second != DrumKit::Silence)
        {
            auto lines = helper.getDrumKitHorizontalLines(kitPosition.second);
            // test that there is one line
            if (lines.size() != 1)
                // the number of possible lines should be 1
                // as a non-silence drumkit has one line
            {

                auto drumKitStr = getSerializedDrumKit(kitPosition.second);
                throw DrumException("Error from 'Drum::DrumTabPart::getDrumKit': '" + drumKitStr
                                    + "' has one or multiple DrumKitHorizontalLines. "
                                    + std::to_string(lines.size()) + " found.");
            }

            if(line == lines[0])
            {
                return kitPosition.second;
            }
        }
    }

    return DrumKit::Silence;
}

void DrumTabPart::clearLineAtPosition(DrumKitHorizontalLine line, unsigned position)
{
    // do nothing if we ask to erase the None line
    if (line == DrumKitHorizontalLine::None)
    {
        return;
    }


    DrumKitHorizontalLineHelper helper;
    auto possibleDrumKitsAtLine = helper.getDrumKits(line);

    auto itToErase = m_drumKitPositions.cend();

    for(auto it = m_drumKitPositions.cbegin();
        it != m_drumKitPositions.cend();
        it++)
    {
        if (it->first == position)
        {
            bool isSimilar(false);
            unsigned index(0);
            unsigned size(possibleDrumKitsAtLine.size());
            while (isSimilar == false && index < size) {
                isSimilar = it->second == possibleDrumKitsAtLine[index];
                index++;
            }
            // remove the element if it is similar
            if (isSimilar)
                itToErase = it;

        }
    }
    if (itToErase != m_drumKitPositions.cend())
        m_drumKitPositions.erase(itToErase);

    m_isDrumLinesUptoDate = false;
}

void DrumTabPart::fillSerializer(Tools::SerializerHelper &helper) const
{
    //setting
    helper.addToSerializeString(TIME,m_drumTime);

    // drum Kits serialization
    auto& drumKitsHelper = helper.addSubSerializerHelper(DRUMKITS);
    for (unsigned indexDrumKit(0); indexDrumKit < m_drumKitPositions.size(); indexDrumKit++)
    {
        auto& helperForDrumKit = drumKitsHelper.addSubSerializerHelper(std::to_string(indexDrumKit));
        helperForDrumKit.addToSerializeString(POSITION,m_drumKitPositions[indexDrumKit].first);
        helperForDrumKit.addToSerializeString(KIT,getSerializedDrumKit(m_drumKitPositions[indexDrumKit].second));
    }
}

void DrumTabPart::fillFromSerialized(const SerializerHelper &helper)
{
    helper.deserialize(TIME,m_drumTime);

    if (helper.isSerializerHelper(DRUMKITS) == false)
        return;


    auto& kitsHelper = helper.getSubSerializerHelper(DRUMKITS);
    unsigned indexKit(0);
    while(true)
    {
        // stop if there are no more helper
        auto& currentKitHelper = kitsHelper.getSubSerializerHelper(std::to_string(indexKit));
        if (&currentKitHelper == &kitsHelper)
            break;

        // create a new drum tab part
        unsigned drumKitPosition(0);
        std::string drumKitStr("");
        currentKitHelper.deserialize(KIT,drumKitStr);
        currentKitHelper.deserialize(POSITION,drumKitPosition);

        addDrumKit(getDrumKit(drumKitStr),drumKitPosition);

        indexKit++;
    }


}

// =======================
// == Private Functions ==
// =======================

bool DrumTabPart::isFloorDrumKit(DrumKit drumKit) const
{
    return drumKit == DrumKit::BassDrum || drumKit == DrumKit::HitHatFloor;
}

void DrumTabPart::deleteDrumLines()
{
    for(auto* drumLine : m_drumLines)
    {
        delete drumLine;
    }
    m_drumLines = {};
}

void DrumTabPart::createDrumLines()
{
    if(m_isDrumLinesUptoDate)
        return;

    // we need first to sort the drum kits with the position
    sortDrumKits();

    // empty the previous drum lines
    deleteDrumLines();

    // croche line creation
    // --------------------
    if(m_drumTime%4 == 0)
    {
        // Drum lines under creation
        DrumLine* TopLine = new DrumLine();
        DrumLine* FloorLine = new DrumLine();
        int previousFloorDrumLineCategory(-1);
        int previousTopDrumLineCategory(-1);

        for(const auto& drumKitPositionPair : m_drumKitPositions)
        {
            bool isFloor = isFloorDrumKit(drumKitPositionPair.second);

            unsigned position = drumKitPositionPair.first;

            int currentDrumLineCategory(std::ceil(static_cast<double>(position)/
                                                  static_cast<double>(m_drumTime)*
                                                  4.0));


            // start a new drum line
            if(isFloor == true
            && currentDrumLineCategory != previousFloorDrumLineCategory)
            {

                // save the previous one
                if(FloorLine->isEmpty() == false && FloorLine->getEndPosition() != FloorLine->getStartPosition())
                {
                    m_drumLines.push_back(FloorLine);
                }

                FloorLine = new DrumLine(); // create an empty DrumLine
                FloorLine->setIsFloor(isFloor);
                FloorLine->setDrumLineType(DrumLine::DrumLineType::Croche);
                FloorLine->setStartPosition(position);
                FloorLine->setEndPosition(position);
                previousFloorDrumLineCategory = currentDrumLineCategory;
            }
            else if(isFloor == false
                 && currentDrumLineCategory != previousTopDrumLineCategory)
            {

                // save the previous one
                if(TopLine->isEmpty() == false && TopLine->getEndPosition() != TopLine->getStartPosition())
                {
                    m_drumLines.push_back(TopLine);
                }

                TopLine = new DrumLine(); // create an empty DrumLine
                TopLine->setIsFloor(isFloor);
                TopLine->setDrumLineType(DrumLine::DrumLineType::Croche);
                TopLine->setStartPosition(position);
                TopLine->setEndPosition(position);
                previousTopDrumLineCategory = currentDrumLineCategory;
            }
            // update a drum line
            else if(isFloor == true
                 && currentDrumLineCategory == previousFloorDrumLineCategory)
            {
                FloorLine->setEndPosition(position);
            }


            else if(isFloor == false
                 && currentDrumLineCategory == previousTopDrumLineCategory)
            {
                TopLine->setEndPosition(position);
            }

        }

        // save the last one
        if(FloorLine->isEmpty() == false && FloorLine->getEndPosition() != FloorLine->getStartPosition())
        {
            m_drumLines.push_back(FloorLine);
        }
        else
        {
            delete FloorLine;
        }
        if(TopLine->isEmpty() == false && TopLine->getEndPosition() != TopLine->getStartPosition())
        {
            m_drumLines.push_back(TopLine);
        }
        else
        {
            delete TopLine;
        }
    }

}

const std::vector<std::pair<DrumKit,std::string>> DrumTabPart::s_stringDrumkitTranslation =
{
    std::make_pair<DrumKit,std::string>(DrumKit::BassDrum,"BassDrum"),
    std::make_pair<DrumKit,std::string>(DrumKit::FloorTom,"FloorTom"),
    std::make_pair<DrumKit,std::string>(DrumKit::SnareTom,"SnareTom"),
    std::make_pair<DrumKit,std::string>(DrumKit::LowTom,"LowTom"),
    std::make_pair<DrumKit,std::string>(DrumKit::HighTom,"HighTom"),
    std::make_pair<DrumKit,std::string>(DrumKit::HiHatClosed,"HiHatClosed"),
    std::make_pair<DrumKit,std::string>(DrumKit::HiHatOpened,"HiHatOpened"),
    std::make_pair<DrumKit,std::string>(DrumKit::Crash,"Crash"),
    std::make_pair<DrumKit,std::string>(DrumKit::RideBell,"RideBell"),
    std::make_pair<DrumKit,std::string>(DrumKit::Ride,"Ride"),
    std::make_pair<DrumKit,std::string>(DrumKit::HitHatFloor,"HitHatFloor"),
    std::make_pair<DrumKit,std::string>(DrumKit::Silence,"Silence")
};

DrumKit DrumTabPart::getDrumKit(const std::string& serializeddrumKit) const
{

    DrumKit returnedDrumKit;
    bool found(false);
    unsigned index(0);
    unsigned size(s_stringDrumkitTranslation.size());
    while(index < size && found == false)
    {
        if(serializeddrumKit == s_stringDrumkitTranslation[index].second)
        {
            returnedDrumKit = s_stringDrumkitTranslation[index].first;
            found = true;
        }
        index++;
    }

    if(found == false)
    {
        throw DrumException("Error from 'getDrumKit' : '" + serializeddrumKit
                          + "' is not recognized as DrumKit.");
    }

    return returnedDrumKit;


}

std::string DrumTabPart::getSerializedDrumKit(DrumKit drumKit) const
{
    std::string returnedSerialized;
    bool found(false);
    unsigned index(0);
    unsigned size(s_stringDrumkitTranslation.size());
    while(index < size && found == false)
    {
        if(drumKit == s_stringDrumkitTranslation[index].first)
        {
            returnedSerialized = s_stringDrumkitTranslation[index].second;
            found = true;
        }
        index++;
    }

    if(found == false)
    {
        throw DrumException("Error from 'DrumTabPart::getSerialized' : impossible to find the Drumkit.");
    }

    return returnedSerialized;

}
