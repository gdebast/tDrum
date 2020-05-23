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
        Drum::DrumTabPart *currentDrumTabPart = it->first;

        // explicit/implicit flag
        // ----------------------
        // the drum tab part is explicit so we simply copy it
        // the first drum tab part is obviously explicit
        if (it->second == false || it == drumRabParts_bool.cbegin() || currentDrumTabPart->isEmpty())
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

            currentTabToFollow.push_back(currentDrumTabPart);
            continue;
        }

        // find the implicit and explicit part
        // -----------------------------------
        std::vector<Drum::DrumTabPart*> implicitPart{};
        std::list<Drum::DrumTabPart*> explicitPart{};
        auto backwardIt = currentTabToFollow.rbegin();
        bool continueFlag(true);
        while(backwardIt != currentTabToFollow.rend() &&
              continueFlag &&
              it != drumRabParts_bool.cend())
        {

            DrumTabPart& previousPart = **backwardIt;
            DrumTabPart& nextPart = *(it->first);

            // continue only if the part is flagged as implicit and is identical to the previous
            continueFlag = it->second && previousPart == nextPart;
            if (continueFlag)
            {

                implicitPart.push_back(&nextPart);
                explicitPart.push_front(&previousPart);
                it++;
                backwardIt++;
            }


        }

        // assert
        DrumException::drumAssert(implicitPart.size() == explicitPart.size(),
                                  "Error from 'DrumTabImpliciter::getImplicit' : "
                                  "the size of both explicit and implicit parts are different."
                                  "Got {} and {}.",
                                  std::to_string(implicitPart.size()),
                                  std::to_string(explicitPart.size()));

        DrumException::drumAssert(currentTabToFollow.size() >= explicitPart.size(),
                                  "Error from 'DrumTabImpliciter::getImplicit' : "
                                  "the size of currentTabToFollow is smaller than explicitPart."
                                  "Got {} and {}.",
                                  std::to_string(currentTabToFollow.size()),
                                  std::to_string(explicitPart.size()));


        // treat the explicit and implicit part
        // ------------------------------------
        if (implicitPart.size() == 0) // an empty implicit part means that we have to
                                      // put the current part in the current parts to follow
        {
            currentTabToFollow.push_back(it->first);
        }
        else if (explicitPart.size() == currentTabToFollow.size()) // if both part to follow and explicit are indentical (checked by the size)
                                                                   // we can simply add 1 to the number of implicit
        {
            currentTabToFollowRepetition++;
        }
        else // otherwise it means that we have to cut the current Tab to follow
        {
            unsigned sizeOfFirstTabToFollow = currentTabToFollow.size() - explicitPart.size();
            std::vector<Drum::DrumTabPart*> firstTabToFollow{};
            std::vector<Drum::DrumTabPart*> secondTabToFollow{};
            firstTabToFollow.reserve(sizeOfFirstTabToFollow);
            secondTabToFollow.reserve(explicitPart.size());

            for(unsigned index(0); index < currentTabToFollow.size(); index++)
            {
                if (index < sizeOfFirstTabToFollow)
                {
                    firstTabToFollow.push_back(currentTabToFollow[index]);
                }
                else
                {
                    secondTabToFollow.push_back(currentTabToFollow[index]);
                }
            }

            returnedImplicitTab.push_back(std::make_pair(std::move(firstTabToFollow),currentTabToFollowRepetition));

            currentTabToFollow = std::move(secondTabToFollow);
            currentTabToFollowRepetition = 1;

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
