#include "DrumAPI/drum_drumtabimpliciter.h"

#include "DrumAPI/drum_drumtabpart.h"

using namespace Drum;

DrumTabImpliciter::DrumTabImpliciter()
{

}

DrumTabImpliciter::DrumTabPartRepetition
DrumTabImpliciter::getImplicit(const std::vector<std::pair<Drum::DrumTabPart *, bool> > &drumRabParts_bool) const
{
    DrumTabImpliciter::DrumTabPartRepetition  returnedImplicitTab;
    std::vector<Drum::DrumTabPart*> currentTabToFollow{};
    unsigned currentTabToFollowRepetition(1);

    for (auto it =  drumRabParts_bool.cbegin(); it != drumRabParts_bool.cend(); it++)
    {

        // the drum tab part is explicit so we simply copy it
        // the first drum tab part is obviously explicit
        if (it->second == false || it == drumRabParts_bool.cbegin())
        {
            // if the previous tab to follow has a repetition of more than once,
            // we have to save it to the returned vector
            if (currentTabToFollowRepetition > 1 && currentTabToFollow.empty() == false)
            {
                returnedImplicitTab.push_back(std::make_pair(currentTabToFollow,currentTabToFollowRepetition));

                // prepare for the next
                currentTabToFollow.clear();
                currentTabToFollowRepetition = 1;

            }

            currentTabToFollow.push_back(it->first);
            continue;
        }

        // the drum tab part is implicit,
        // we have to test that the previous parts are identical

    }

    // create the last
    if (currentTabToFollowRepetition > 1 && currentTabToFollow.empty() == false)
    {
        returnedImplicitTab.push_back(std::make_pair(currentTabToFollow,currentTabToFollowRepetition));
    }

    return returnedImplicitTab;
}

DrumTabImpliciter::DrumTabPartRepetition
DrumTabImpliciter::getExplicit(const std::vector<std::pair<DrumTabPart *, bool> > &drumRabParts_bool) const
{
    std::vector<Drum::DrumTabPart*> currentTabToFollow{};
    for (const auto& pair : drumRabParts_bool)
    {
        auto* drumTabPart = pair.first;
        currentTabToFollow.push_back(drumTabPart);
    }

    return DrumTabImpliciter::DrumTabPartRepetition{std::make_pair(currentTabToFollow,1)};
}
