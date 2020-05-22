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
            void synchronizeModel() const;
        public slots:
            void setExplicitDrumTabPrinting(bool explicitPrinting){m_explicitDrumTabPrinting = explicitPrinting;};
            void setPdfExportDirector(const QString& directory){m_pdfExportDirectory = directory;};


        private:
            Drum::DrumTabPdfPrinterConfig& m_model;

            bool    m_explicitDrumTabPrinting{true};
            QString m_pdfExportDirectory{""};


    };

}
