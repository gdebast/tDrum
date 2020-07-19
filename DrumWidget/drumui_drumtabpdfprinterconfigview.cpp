#include "DrumWidget/drumui_drumtabpdfprinterconfigview.h"

#include "DrumWidget/drumui_drumtabpdfprinterconfigviewmodel.h"
#include "UI/ui_directorylineedit.h"

#include <map>

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
    // explicit/implicit printing
    m_explicitImplicitCheckBox = new QCheckBox("Explicit drum tab printing",this);
    if (m_viewModel.isDrumTabPrintingExplicit())
        m_explicitImplicitCheckBox->setCheckState(Qt::CheckState::Checked);
    else
        m_explicitImplicitCheckBox->setCheckState(Qt::CheckState::Unchecked);

    // export directory
    m_directoryLineEdit = new UI::DirectoryLineEdit(m_viewModel.getPdfExportDirectory(),this);

    // number of parts per row
    m_qComboBox_NumberOfPartPerRow = new QComboBox(this);
    m_qLabel_NumberOfPartPerRow = new QLabel("Parts per row:",this);
    std::map<unsigned,QString> possibleValues = {{3,QString("3")},{4,QString("4")},{5,QString("5")}};
    m_qStandardItemModel_NumberOfPartPerRow = new QStandardItemModel(this);
    unsigned index(0);
    unsigned currentIndex(0);
    for(const auto& [number,text] : possibleValues)
    {
        QVariant value(number);
        QStandardItem* item = new QStandardItem(text);
        item->setData(value);
        m_qStandardItemModel_NumberOfPartPerRow->appendRow(item);
        if(m_viewModel.getNumberOfDrumTabPartsPerRow() == number)
        {
            currentIndex = index;
        }
        index++;

    }
    m_qComboBox_NumberOfPartPerRow->setModel(m_qStandardItemModel_NumberOfPartPerRow);
    m_qComboBox_NumberOfPartPerRow->setCurrentIndex(currentIndex);

    // ok button
    m_qPushButton_ok = new QPushButton("Ok",this);

}

void DrumTabPdfPrinterConfigView::createLayout()
{
    m_mainLayout = new QVBoxLayout(this);
    m_NumberOfPartPerRowLayout = new QHBoxLayout();
}

void DrumTabPdfPrinterConfigView::addWidgetToLayout()
{
    m_mainLayout->addWidget(m_explicitImplicitCheckBox);
    m_mainLayout->addLayout(m_NumberOfPartPerRowLayout);
    m_NumberOfPartPerRowLayout->addWidget(m_qLabel_NumberOfPartPerRow);
    m_NumberOfPartPerRowLayout->addWidget(m_qComboBox_NumberOfPartPerRow);
    m_NumberOfPartPerRowLayout->addStretch();
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

    QObject::connect(m_qComboBox_NumberOfPartPerRow,
                     QOverload<int>::of(&QComboBox::currentIndexChanged),
                     &m_viewModel,
                     [this](int index)
                        {
                            auto* item = m_qStandardItemModel_NumberOfPartPerRow->item(index);
                            unsigned nmbrOfPart = item->data().toInt();
                            m_viewModel.setNumberOfDrumTabPartsPerRow(nmbrOfPart);
                        });

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
