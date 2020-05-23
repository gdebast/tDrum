#include "DrumAPI/drum_drumtabimpliciter.h"

#include "DrumAPI/drum_drumtabpart.h"
#include "DrumAPI/drum_drumexception.h"

#include <list>

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

        //find the implicit and explicit part
        std::vector<Drum::DrumTabPart*> implicitPart{};
        std::list<Drum::DrumTabPart*> explicitPart{};
        auto backwardIt = currentTabToFollow.rbegin();
        bool isIdentical(true);
        while(backwardIt != currentTabToFollow.rend() &&
              isIdentical &&
              it != drumRabParts_bool.cend())
        {

            DrumTabPart& previousPart = **backwardIt;
            DrumTabPart& nextPart = *(it->first);

            isIdentical = previousPart == nextPart;
            if (isIdentical)
            {

                implicitPart.push_back(&nextPart);
                explicitPart.push_front(&previousPart);
                it++;
                backwardIt++;
            }


        }

        // assert
        DrumException::drumAssert(implicitPart.size() != explicitPart.size(),
                                  "Error from 'DrumTabImpliciter::getImplicit' : "
                                  "the size of both explicit and implicit parts are different."
                                  "Got {} and {}.",
                                  std::to_string(implicitPart.size()),
                                  std::to_string(explicitPart.size()));



        // treat the explicit and implicit part
        if (implicitPart.size() == 0) // an empty implicit part means that we have to
                                      // put the current part in the current parts to follow
        {
            currentTabToFollow.push_back(it->first);
            continue;
        }
        else if (explicitPart.size() == currentTabToFollow.size()) // if both part to follow and explicit are indentical (checked by the size)
                                                                   // we can simply add 1 to the number of implicit
        {
            currentTabToFollowRepetition++;
        }
        else // otherwise it means that we have to cut the current Tab to follow
        {
            // TODO
        }


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
