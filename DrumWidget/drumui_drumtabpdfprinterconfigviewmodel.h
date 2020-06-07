#pragma once

#include <QObject>

namespace Drum
{
    class DrumTabPdfPrinterConfig;
}

namespace  DrumUI
{
    class DrumTabPdfPrinterConfigViewModel : public QObject
    {
        Q_OBJECT
        public:
            explicit DrumTabPdfPrinterConfigViewModel(Drum::DrumTabPdfPrinterConfig& model);
            bool isDrumTabPrintingExplicit() const {return m_explicitDrumTabPrinting;}
            QString getPdfExportDirectory() const {return m_pdfExportDirectory;}
            unsigned getNumberOfDrumTabPartsPerRow(){return m_numberOfDrumTabPartsPerRow;};
            void synchronizeModel() const;
        public slots:
            void setExplicitDrumTabPrinting(bool explicitPrinting){m_explicitDrumTabPrinting = explicitPrinting;};
            void setPdfExportDirector(const QString& directory){m_pdfExportDirectory = directory;};
            void setNumberOfDrumTabPartsPerRow(unsigned numberOfDrumTabPartsPerRow){m_numberOfDrumTabPartsPerRow = numberOfDrumTabPartsPerRow;};


        private:
            Drum::DrumTabPdfPrinterConfig& m_model;

            bool     m_explicitDrumTabPrinting{true};
            QString  m_pdfExportDirectory{""};
            unsigned m_numberOfDrumTabPartsPerRow{4};


    };

}
