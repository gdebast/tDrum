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
    std::vector<Drum::DrumTabPart*> currenttabtoFollow{};

    for (auto it =  drumRabParts_bool.cbegin(); it != drumRabParts_bool.cend(); it++)
    {



        // the drum tab part is explicit so we simply copy it
        // the first drum tab part is obviously explicit
        if (it->second == false || it == drumRabParts_bool.cbegin())
        {
            currenttabtoFollow.push_back(it->first);
            continue;
        }

        // the drum tab part is implicit,
        // we have to test that the previous part are identical

    }
}
