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
         * this type defines a list of drum tab parts with the number of type it is repeated.
         */
        using DrumTabPartRepetition = std::vector<std::pair<std::vector<Drum::DrumTabPart*> , unsigned> >;

        /*
         * This method reads the vector of Drum tab parts (with its implicit/explicit bool flag)
         * and returns the implicit version. If a part is flagged as implicit, the algorithm can
         * detect that the part should be repeated with its previous, hence increasing the number
         * of repetitions.
         * [input] drumRabParts_bool, vector of pair of DrumTabPart with its implicit(true)/explicit(false) flag
         */
        DrumTabPartRepetition
        getImplicit(const std::vector<std::pair<DrumTabPart*,bool>>& drumRabParts_bool) const;

        /*
         * This method reads the vector of Drum tab parts and returns a DrumTabPartRepetition
         * with this vector, and 1, as first abnd only pair. the booleans in the input are thus
         * not read.
         */
        DrumTabPartRepetition
        getExplicit(const std::vector<std::pair<DrumTabPart*,bool>>& drumRabParts_bool) const;


    };
}

