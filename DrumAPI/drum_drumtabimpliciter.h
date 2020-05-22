#pragma once

#include <vector>
#include <utility>

namespace Drum
{

    class DrumTabPart;

    class DrumTabImpliciter
    {
    public:
        DrumTabImpliciter();

        /*
         * this method return a vector of vector of DrumTabPart followed by
         * the number of times this vector of DrumTabPart is repeated.
         * The input are pairs of DrumTabPart and bool (implicit=true, explicit=false).
         * An implicit DrumTabPart can be potentialy implicit
         */
        std::vector<std::pair<std::vector<Drum::DrumTabPart*> , unsigned> >
        getImplicit(const std::vector<std::pair<DrumTabPart*,bool>>& drumRabParts_bool) const;

        /*
         * this method return a vector of vector of DrumTabPart followed by
         * the number of times this vector of DrumTabPart is repeated.
         * This is explicit version so in input vector will be copied in the first pair
         * with the number 1, meaning the entire vector is present once.
         */
        std::vector<std::pair<std::vector<Drum::DrumTabPart *>, unsigned> >
        getExplicit(const std::vector<std::pair<DrumTabPart*,bool>>& drumRabParts_bool) const;


    };
}

