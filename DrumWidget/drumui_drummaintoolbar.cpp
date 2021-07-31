#include "DrumWidget/drumui_drummaintoolbar.h"

#include <DrumAPI/drum_drumtab.h>
#include <DrumAPI/drum_drumtabpdfprinterconfig.h>
#include "DrumWidget/drumui_drumtabpdfprinterconfigviewmodel.h"
#include "DrumWidget/drumui_drumtabpdfprinterconfigview.h"
#include <UI/ui_rightleftpushbutton.h>

using namespace DrumUI;

DrumMainToolBar::DrumMainToolBar(Drum::DrumTab *drumTab,
                                 Drum::DrumTabPdfPrinterConfig& pdfConfig,
                                 const Tools::DirectoryManager& directoryManager,
                                 QWidget *parent) :
    QWidget(parent),
    m_drumTab(drumTab),
    m_drumTabPdfPrinterConfig(pdfConfig),
    m_drumTabPdfCreator(drumTab,pdfConfig),
    m_directoryManager(directoryManager)
{
    createWidget();
    setDrumTab(drumTab);
    createLayout();
    addWidgetToLayout();
    connectWidget();
}

void DrumMainToolBar::setDrumTab(Drum::DrumTab *drumTab)
{
    m_drumTab = drumTab;

    QString title(m_drumTab ? m_drumTab->getTitle().c_str() : "");
    QString author(m_drumTab ? m_drumTab->getAuthor().c_str() : "");

    m_qLineEdit_Author->setText(author);
    m_qLineEdit_Title->setText(title);

    m_qLineEdit_Author->setEnabled(m_drumTab);
    m_qLineEdit_Title->setEnabled(m_drumTab);

    m_drumTabPdfCreator.setDrumTab(m_drumTab);

}

void DrumMainToolBar::createWidget()
{

    QSizePolicy fixedPolicy(QSizePolicy::Policy::Fixed,QSizePolicy::Policy::Fixed);
    QSizePolicy ExpendingPolicyHor(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Fixed);
    QFont font("Times", 20);

    m_qLineEdit_Author = new QLineEdit(this);

    m_qLineEdit_Author->setFont(font);
    m_qLineEdit_Author->setSizePolicy(fixedPolicy);
    m_qLineEdit_Title = new QLineEdit(this);

    m_qLineEdit_Title->setFont(font);
    m_qLineEdit_Title->setSizePolicy(ExpendingPolicyHor);

    m_rightLeftPushButton_exportToPdf = new UI::RightLeftPushButton(this);
    m_rightLeftPushButton_exportToPdf->setIcon(QIcon(":/tDrum_createPdf_giveTree.svg"));
    m_rightLeftPushButton_exportToPdf->setToolTip("Export to Pdf");
    m_rightLeftPushButton_exportToPdf->setSizePolicy(fixedPolicy);
}

void DrumMainToolBar::createLayout()
{
    m_qHBoxLayout_mainLayout = new QHBoxLayout(this);
    setLayout(m_qHBoxLayout_mainLayout);
}

void DrumMainToolBar::addWidgetToLayout()
{
    m_qHBoxLayout_mainLayout->addWidget(m_rightLeftPushButton_exportToPdf);
    m_qHBoxLayout_mainLayout->addWidget(m_qLineEdit_Author);
    m_qHBoxLayout_mainLayout->addWidget(m_qLineEdit_Title);
}

void DrumMainToolBar::connectWidget()
{
    QObject::connect(m_qLineEdit_Title,
                     &QLineEdit::textChanged,
                     this,
                     [this](const QString &text)
                     {
                        if (m_drumTab == nullptr)
                            return;

                        auto &drumTab(*m_drumTab);
                        drumTab.setTitle(text.toStdString());
                        emit drumTabChanged(drumTab);
                     });

    QObject::connect(m_qLineEdit_Author,
                     &QLineEdit::textChanged,
                     this,
                     [this](const QString &text)
                     {
                        if (m_drumTab == nullptr)
                            return;

                        auto &drumTab(*m_drumTab);
                        drumTab.setAuthor(text.toStdString());
                        emit drumTabChanged(drumTab);
                     });

    QObject::connect(m_rightLeftPushButton_exportToPdf,
                     &UI::RightLeftPushButton::leftClick,
                     this,
                     [this]()
                     {
                        m_drumTabPdfCreator.createPdf();
                     });

    QObject::connect(m_rightLeftPushButton_exportToPdf,
                     &UI::RightLeftPushButton::rightClicked,
                     this,
                     [this]()
                     {
                        editDrumTabPdfPrinterConfig();
                     });

}

void DrumMainToolBar::editDrumTabPdfPrinterConfig()
{
    // create a viewModel for the DrumTabPdfPrinterConfig
    DrumTabPdfPrinterConfigViewModel viewModel(m_drumTabPdfPrinterConfig);

    // create the view
    DrumTabPdfPrinterConfigView view(viewModel,m_directoryManager,this);
    view.move(mapToGlobal(QPoint(m_rightLeftPushButton_exportToPdf->x(),m_rightLeftPushButton_exportToPdf->y())));
    view.exec();
}
