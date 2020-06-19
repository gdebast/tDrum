#pragma once

#include "DrumAPI/drum_idrumfactory.h"
#include "DrumAPI/drum_drumtabpdfprinterconfig.h"

#include <string>

namespace Drum
{
    class DrumTabPdfPrinterConfigFactory : public IDrumFactory<DrumTabPdfPrinterConfig>
    /*
     * class responsible for loading, owning and saving DrumTabPdfPrinterConfig
     */
    {
        public:
            DrumTabPdfPrinterConfigFactory(const Tools::Directory &workingDirectory);
            DrumTabPdfPrinterConfigFactory(const DrumTabPdfPrinterConfigFactory&) = delete;
            DrumTabPdfPrinterConfigFactory(DrumTabPdfPrinterConfigFactory&&) = delete;
            DrumTabPdfPrinterConfigFactory& operator=(const DrumTabPdfPrinterConfigFactory&) = delete;
            DrumTabPdfPrinterConfigFactory& operator=(DrumTabPdfPrinterConfigFactory&&) = delete;
            ~DrumTabPdfPrinterConfigFactory();

            DrumTabPdfPrinterConfig &getOneObject() final;

    };
}


