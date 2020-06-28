#pragma once

#include "DrumAPI/drum_drumtabpart.h"
#include "DrumAPI/drum_idrumfactory.h"

#include <string>

namespace Drum {

    /*
     * class responsible for creating, saving, and importing
     * the standalone DrumTabParts
     */
    class DrumTabPartFactory final : public IDrumFactory<DrumTabPart>
    {
    public:
        explicit DrumTabPartFactory(const Tools::Directory &workingDirectory);
        DrumTabPartFactory(const DrumTabPartFactory&) = delete;
        DrumTabPartFactory(DrumTabPartFactory&&) = delete;
        DrumTabPartFactory& operator=(const DrumTabPartFactory&) = delete;
        DrumTabPartFactory& operator=(DrumTabPartFactory&&) = delete;
        ~DrumTabPartFactory();

        DrumTabPart &createDefaultObject() final;


    };
}

