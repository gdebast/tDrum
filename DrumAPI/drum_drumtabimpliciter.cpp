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

    for (auto it =  drumRabParts_bool.cbegin(); it != drumRabParts_bool.cend();)
    {
        Drum::DrumTabPart *currentDrumTabPart = it->first;
        DrumTabPart& currentDrumTabPartRef = *currentDrumTabPart;

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
                returnedImplicitTab.push_back(std::make_pair(std::move(currentTabToFollow),currentTabToFollowRepetition));

                // prepare for the next
                currentTabToFollow = {};
                currentTabToFollowRepetition = 1;

            }

            currentTabToFollow.push_back(currentDrumTabPart);
            it++;
            continue;
        }
        auto itCopy = it;

        // find the first occurence, backward
        // ----------------------------------
        int index_currentTabToFollow(currentTabToFollow.size()-1);
        bool found(false);
        while (index_currentTabToFollow >= 0 && found == false)
        {
            DrumTabPart& DrumTabPartIn_currentTabToFollow = *currentTabToFollow[index_currentTabToFollow];
            bool identical = DrumTabPartIn_currentTabToFollow == currentDrumTabPartRef;
            if(identical)
            {
                found = true;
            }
            else
            {
                index_currentTabToFollow--;
            }
        }
        auto forwardIt = currentTabToFollow.end();
        if(found && index_currentTabToFollow >= 0)
        {
            forwardIt = currentTabToFollow.begin()+index_currentTabToFollow;
        }

        // find the implicit and explicit part
        // -----------------------------------
        unsigned implicitPart_size(0);
        unsigned explicitPart_size(0);
        bool continueFlag(true);
        while(forwardIt != currentTabToFollow.end() &&
              continueFlag &&
              it != drumRabParts_bool.cend())
        {

            DrumTabPart& previousPart = **forwardIt;
            DrumTabPart& nextPart = *(it->first);

            // continue only if the part is flagged as implicit and is identical to the previous
            continueFlag = it->second && previousPart == nextPart;
            if (continueFlag)
            {

                implicitPart_size++;
                explicitPart_size++;
                it++;
                forwardIt++;
            }


        }

        // assert
        DrumException::drumAssert(implicitPart_size == explicitPart_size,
                                  "Error from 'DrumTabImpliciter::getImplicit' : "
                                  "the size of both explicit and implicit parts are different."
                                  "Got {} and {}.",
                                  implicitPart_size,
                                  explicitPart_size);

        DrumException::drumAssert(currentTabToFollow.size() >= explicitPart_size,
                                  "Error from 'DrumTabImpliciter::getImplicit' : "
                                  "the size of currentTabToFollow is smaller than explicitPart."
                                  "Got {} and {}.",
                                  std::to_string(currentTabToFollow.size()),
                                  explicitPart_size);


        // treat the explicit and implicit part
        // ------------------------------------
        if (implicitPart_size == 0) // an empty implicit part means that we have to
                                    // put the current part in the current parts to follow
                                    // because the impliciter did not find a repetition.
        {
            if (currentTabToFollowRepetition > 1)
            {
                returnedImplicitTab.push_back(std::make_pair(std::move(currentTabToFollow),currentTabToFollowRepetition));
                currentTabToFollow = {};
                currentTabToFollowRepetition = 1;

            }
            currentTabToFollow.push_back(it->first);
            it++;
        }
        else if (forwardIt != currentTabToFollow.end())
        {
            currentTabToFollow.push_back(itCopy->first);
            it = itCopy+1;
        }
        else if (explicitPart_size == currentTabToFollow.size()) // if both part to follow and explicit are indentical (checked by the size)
                                                                 // we can simply add 1 to the number of implicit
        {
            currentTabToFollowRepetition++;
        }
        else // otherwise it means that we have to cut the current Tab to follow
        {
            unsigned sizeOfFirstTabToFollow = currentTabToFollow.size() - explicitPart_size;
            std::vector<Drum::DrumTabPart*> firstTabToFollow{};
            std::vector<Drum::DrumTabPart*> secondTabToFollow{};
            firstTabToFollow.reserve(sizeOfFirstTabToFollow);
            secondTabToFollow.reserve(explicitPart_size);

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
            currentTabToFollowRepetition = 2;

        }


    }

    // create the last
    if (currentTabToFollowRepetition > 0 && currentTabToFollow.empty() == false)
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
