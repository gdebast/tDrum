#pragma once

#include <QDialog>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QStandardItemModel>

namespace  UI
{
    class DirectoryLineEdit;
}

namespace  DrumUI
{
    class DrumTabPdfPrinterConfigViewModel;
}

namespace  DrumUI
{
    /*
     * view of DrumTabPdfPrinterConfig
     */

    class DrumTabPdfPrinterConfigView : public QDialog
    {
        Q_OBJECT
        public:
            explicit DrumTabPdfPrinterConfigView(DrumTabPdfPrinterConfigViewModel& viewModel,
                                                 QWidget *parent = nullptr);

        private:

            void createWidget();
            void createLayout();
            void addWidgetToLayout();
            void connectWidget();

            DrumTabPdfPrinterConfigViewModel& m_viewModel;

            QCheckBox*             m_explicitImplicitCheckBox{nullptr};
            UI::DirectoryLineEdit* m_directoryLineEdit{nullptr};
            QPushButton*           m_qPushButton_ok{nullptr};
            QVBoxLayout*           m_mainLayout{nullptr};
            QHBoxLayout*           m_NumberOfPartPerRowLayout{nullptr};
            QComboBox*             m_qComboBox_NumberOfPartPerRow{nullptr};
            QLabel*                m_qLabel_NumberOfPartPerRow{nullptr};
            QStandardItemModel*    m_qStandardItemModel_NumberOfPartPerRow{nullptr};


    };
}

