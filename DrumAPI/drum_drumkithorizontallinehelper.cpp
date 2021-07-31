#include "DrumAPI/drum_drumkithorizontallinehelper.h"

using namespace Drum;

DrumKitHorizontalLineHelper::DrumKitHorizontalLineHelper()
{
}

std::vector<DrumKit> DrumKitHorizontalLineHelper::getDrumKits(DrumKitHorizontalLine line) const
{
    std::vector<DrumKit> returnDrumKits{};
    for(const auto &pair : s_drumKitDrumKitHorizontalLineCombs)
    {
        if (pair.second == line)
            returnDrumKits.push_back(pair.first);
    }
    return returnDrumKits;
}

std::vector<DrumKitHorizontalLine> DrumKitHorizontalLineHelper::getDrumKitHorizontalLines(DrumKit drumKit) const
{
    std::vector<DrumKitHorizontalLine> returnDrumKitHorizontalLines{};
    for(const auto &pair : s_drumKitDrumKitHorizontalLineCombs)
    {
        if (pair.first == drumKit)
            returnDrumKitHorizontalLines.push_back(pair.second);
    }
    return returnDrumKitHorizontalLines;
}

std::vector<std::pair<DrumKit,DrumKitHorizontalLine>> DrumKitHorizontalLineHelper::s_drumKitDrumKitHorizontalLineCombs
{
    // Crash line
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::Crash,
                DrumKitHorizontalLine::CrashLine),
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::Ride,
                DrumKitHorizontalLine::CrashLine),
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::RideBell,
                DrumKitHorizontalLine::CrashLine),
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::Silence,
                DrumKitHorizontalLine::CrashLine),

    // hi-hat line
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::HiHatClosed,
                DrumKitHorizontalLine::HiHatLine),
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::HiHatOpened,
                DrumKitHorizontalLine::HiHatLine),
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::Silence,
                DrumKitHorizontalLine::HiHatLine),

    // snare line
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::SnareTom,
                DrumKitHorizontalLine::SnareTomLine),
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::DoubleSnareTom,
                DrumKitHorizontalLine::SnareTomLine),
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::Silence,
                DrumKitHorizontalLine::SnareTomLine),

    // low tom line
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::LowTom,
                DrumKitHorizontalLine::LowTomLine),
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::Silence,
                DrumKitHorizontalLine::LowTomLine),


    // high tom line
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::HighTom,
                DrumKitHorizontalLine::HighTomLine),
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::Silence,
                DrumKitHorizontalLine::HighTomLine),

    // bass line
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::BassDrum,
                DrumKitHorizontalLine::BassDrumLine),
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::Silence,
                DrumKitHorizontalLine::BassDrumLine),

    // floor tom line
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::FloorTom,
                DrumKitHorizontalLine::FloorTomLine),
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::DoubleFloorTom,
                DrumKitHorizontalLine::FloorTomLine),
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::Silence,
                DrumKitHorizontalLine::FloorTomLine),

    // hitHat floor line
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::HitHatFloor,
                DrumKitHorizontalLine::HiHatFloorLine),
    std::make_pair<DrumKit,DrumKitHorizontalLine>(
                DrumKit::Silence,
                DrumKitHorizontalLine::HiHatFloorLine)

};


