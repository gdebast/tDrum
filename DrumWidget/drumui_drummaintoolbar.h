#pragma once

#include "DrumUiTools/drumui_drumtabpdfcreator.h"

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

namespace Drum
{
    class DrumTab;
    class DrumTabPdfPrinterConfig;
}

namespace  UI
{
    class RightLeftPushButton;
}

namespace  Tools
{
    class DirectoryManager;
}

namespace DrumUI
{

    class DrumMainToolBar : public QWidget
    {
        Q_OBJECT
        public:
            explicit DrumMainToolBar(Drum::DrumTab *drumTab, // can be nullprt
                                     Drum::DrumTabPdfPrinterConfig& pdfConfig,
                                     const Tools::DirectoryManager& directoryManager,
                                     QWidget *parent = nullptr);

            void setDrumTab (Drum::DrumTab* drumTab);

        signals:
            void drumTabChanged(Drum::DrumTab &drumTab);

        private:

            void createWidget();
            void createLayout();
            void addWidgetToLayout();
            void connectWidget();

            void editDrumTabPdfPrinterConfig();

            UI::RightLeftPushButton* m_rightLeftPushButton_exportToPdf{nullptr};
            QLineEdit*               m_qLineEdit_Author{nullptr};
            QLineEdit*               m_qLineEdit_Title{nullptr};
            QHBoxLayout*             m_qHBoxLayout_mainLayout{nullptr};

            Drum::DrumTab*                 m_drumTab;
            Drum::DrumTabPdfPrinterConfig& m_drumTabPdfPrinterConfig;
            DrumTabPdfCreator              m_drumTabPdfCreator;

            const Tools::DirectoryManager& m_directoryManager;


    };
}
