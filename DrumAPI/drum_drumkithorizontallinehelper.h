#pragma once

#include "DrumEnum/drum_drumkit.h"
#include "DrumEnum/drum_drumkithorizontalline.h"

#include <vector>
#include <utility>

namespace Drum {

    /*
     * class responsible for giving the correspondance
     * between DrumKit and DrumKitHorizontalLine.
     */

    class DrumKitHorizontalLineHelper
    {
    public:
        DrumKitHorizontalLineHelper();
        // return the possible DrumKit on a line
        std::vector<DrumKit> getDrumKits(DrumKitHorizontalLine line) const;
        // return the DrumKitHorizontalLines (often 1) of a drumKit
        std::vector<DrumKitHorizontalLine> getDrumKitHorizontalLines(DrumKit drumKit) const;


    private:
        static std::vector<std::pair<DrumKit,DrumKitHorizontalLine>> s_drumKitDrumKitHorizontalLineCombs;
};


}


