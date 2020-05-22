#include "DrumAPI/drum_drumtabimpliciter.h"

#include "DrumAPI/drum_drumtabpart.h"

using namespace Drum;

DrumTabImpliciter::DrumTabImpliciter()
{

}

std::vector<std::pair<std::vector<Drum::DrumTabPart*> , unsigned> >
DrumTabImpliciter::getImplicit(const std::vector<std::pair<Drum::DrumTabPart *, bool> > &drumRabParts_bool) const
{
    std::vector<std::pair<std::vector<Drum::DrumTabPart*> , unsigned> >  returnedImplicitTab;
    std::vector<Drum::DrumTabPart*> currentTabToFollow{};

    for (auto it =  drumRabParts_bool.cbegin(); it != drumRabParts_bool.cend(); it++)
    {

        // the drum tab part is explicit so we simply copy it
        // the first drum tab part is obviously explicit
        if (it->second == false || it == drumRabParts_bool.cbegin())
        {
            currentTabToFollow.push_back(it->first);
            continue;
        }

        // the drum tab part is implicit,
        // we have to test that the previous part are identical

    }
}

std::vector<std::pair<std::vector<DrumTabPart *>, unsigned> >
DrumTabImpliciter::getExplicit(const std::vector<std::pair<DrumTabPart *, bool> > &drumRabParts_bool) const
{
    std::vector<Drum::DrumTabPart*> currentTabToFollow{};
    for (const auto& pair : drumRabParts_bool)
    {
        auto* drumTabPart = pair.first;
        currentTabToFollow.push_back(drumTabPart);
    }

    return std::vector<std::pair<std::vector<DrumTabPart *>, unsigned> >{std::make_pair(currentTabToFollow,1)};
}
