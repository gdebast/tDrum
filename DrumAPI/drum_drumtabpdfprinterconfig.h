#pragma once

#include <string>

namespace Drum
{
    class DrumTabPdfPrinterConfig
    {
        public:
            explicit DrumTabPdfPrinterConfig();
            DrumTabPdfPrinterConfig(const DrumTabPdfPrinterConfig&) = delete;
            DrumTabPdfPrinterConfig(DrumTabPdfPrinterConfig&&) = delete;
            DrumTabPdfPrinterConfig& operator=(const DrumTabPdfPrinterConfig&) = delete;
            ~DrumTabPdfPrinterConfig() = default;

            // getter and setters
            bool isDrumTabPrintingExplicit() const {return m_explicitDrumTabPrinting;}
            std::string getPdfExportDirectory() const {return m_pdfExportDirectory;}
            void setExplicitDrumTabPrinting(bool explicitPrinting){m_explicitDrumTabPrinting = explicitPrinting;};
            void setPdfExportDirector(const std::string& directory){m_pdfExportDirectory = directory;};

            // serialization method
            void fillFromSerialized(const std::string &serializedString);
            std::string getSerialized() const;

        private:
            bool        m_explicitDrumTabPrinting{true};
            std::string m_pdfExportDirectory{""};

    };
}

