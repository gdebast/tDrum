#include "DrumWidget/drumui_drumtabpdfprinterconfigview.h"

#include "DrumWidget/drumui_drumtabpdfprinterconfigviewmodel.h"
#include "UI/ui_directorylineedit.h"

using namespace DrumUI;

DrumTabPdfPrinterConfigView::DrumTabPdfPrinterConfigView(DrumTabPdfPrinterConfigViewModel& viewModel,
                                                         QWidget *parent) :
    QDialog(parent),
    m_viewModel(viewModel)
{
    createWidget();
    createLayout();
    addWidgetToLayout();
    connectWidget();
    setWindowIcon(QIcon(":/tDrum_createPdf_giveTree.svg"));
    setWindowTitle("Drum tab pdf export configuration");
    resize(3*width(),height());

}

void DrumTabPdfPrinterConfigView::createWidget()
{
    m_explicitImplicitCheckBox = new QCheckBox("Explicit drum tab printing",this);
    if (m_viewModel.isDrumTabPrintingExplicit())
        m_explicitImplicitCheckBox->setCheckState(Qt::CheckState::Checked);
    else
        m_explicitImplicitCheckBox->setCheckState(Qt::CheckState::Unchecked);
    m_directoryLineEdit = new UI::DirectoryLineEdit(m_viewModel.getPdfExportDirectory(),this);
    m_qPushButton_ok = new QPushButton("Ok",this);
}

void DrumTabPdfPrinterConfigView::createLayout()
{
    m_mainLayout = new QVBoxLayout(this);
}

void DrumTabPdfPrinterConfigView::addWidgetToLayout()
{
    m_mainLayout->addWidget(m_explicitImplicitCheckBox);
    m_mainLayout->addWidget(m_directoryLineEdit);
    m_mainLayout->addWidget(m_qPushButton_ok);
    setLayout(m_mainLayout);
}

void DrumTabPdfPrinterConfigView::connectWidget()
{
    QObject::connect(m_explicitImplicitCheckBox,
                     &QCheckBox::toggled,
                     &m_viewModel,
                     &DrumTabPdfPrinterConfigViewModel::setExplicitDrumTabPrinting);

    QObject::connect(m_directoryLineEdit,
                     &UI::DirectoryLineEdit::directoryChanged,
                     &m_viewModel,
                     &DrumTabPdfPrinterConfigViewModel::setPdfExportDirector);

    QObject::connect(m_qPushButton_ok,
                     &QPushButton::clicked,
                     this,
                     [this]()
                        {
                            m_viewModel.synchronizeModel();
                            close();
                        });
}
