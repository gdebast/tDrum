#pragma once

#include "DrumAPI/drum_idrumfactory.h"
#include "DrumAPI/drum_drumtab.h"

#include <string>

namespace Drum {

/*
     * class responsible for creating, saving, and importing the DrumTab
     */

    class DrumTabFactory final : public IDrumFactory<DrumTab>
    {
    public:
        explicit DrumTabFactory(const Tools::Directory &workingDirectory);
        ~DrumTabFactory();
        DrumTabFactory(const DrumTabFactory&) = delete;
        DrumTabFactory(DrumTabFactory&&) = delete;
        DrumTabFactory& operator=(const DrumTabFactory&) = delete;
        DrumTabFactory& operator=(DrumTabFactory&&) = delete;

        DrumTab &getOneObject() final;


    };
}

