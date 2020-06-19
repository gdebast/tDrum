#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <string>

namespace Drum
{
    class DrumTabPdfPrinterConfig;
}

namespace Drum
{
    class DrumTabPdfPrinterConfigFactory
    /*
     * class responsible for loading, owning and saving DrumTabPdfPrinterConfig
     */
    {
        public:
            DrumTabPdfPrinterConfigFactory();
            DrumTabPdfPrinterConfigFactory(const DrumTabPdfPrinterConfigFactory&) = delete;
            DrumTabPdfPrinterConfigFactory(DrumTabPdfPrinterConfigFactory&&) = delete;
            DrumTabPdfPrinterConfigFactory& operator=(const DrumTabPdfPrinterConfigFactory&) = delete;
            DrumTabPdfPrinterConfigFactory& operator=(DrumTabPdfPrinterConfigFactory&&) = delete;
            ~DrumTabPdfPrinterConfigFactory();

            DrumTabPdfPrinterConfig& loadObject(const std::string& file);
            void dumpToFile() const;

        private:
            std::vector<std::pair<std::unique_ptr<DrumTabPdfPrinterConfig>,std::string>> m_allCreatedObjects;

    };
}


