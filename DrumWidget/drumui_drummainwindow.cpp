#include "DrumWidget/drumui_drummainwindow.h"

#include "DrumAPI/drum_drumtabfactory.h"
#include "DrumAPI/drum_drumtabpartfactory.h"
#include <DrumAPI/drum_drumtabpdfprinterconfigfactory.h>
#include "DrumWidget/drumui_drumtabwidget.h"
#include "DrumWidget/drumui_drumtabpartcreatorwidget.h"
#include "DrumUiTools/drumui_drumtabpdfcreator.h"
#include "DrumWidget/drumui_drummaintoolbar.h"
#include "Tools/tools_directorymanager.h"

using namespace DrumUI;

DrumMainWindow::DrumMainWindow(QWidget *parent) : QMainWindow(parent)
{
    // the project is called tDrum. The author is Gatien De Bast.
    // G is the first later of Gatien and Tea sounds like T.
    // Green is the favorite color of Gatien.
    // -> Green Tea, which is also his favorite beverage.
    setWindowIcon(QIcon(":/tDrum_GreenTea.svg"));

    createWidget();
    createLayout();
    addWidgetToLayout();
    connectWidget();

}

void DrumMainWindow::end()
{
    m_drumTabFactory->dumpToFile();
    m_drumTabPartFactory->dumpToFile();
    m_drumTabPdfPrinterConfigFactory->dumpToFile();
}

DrumMainWindow::~DrumMainWindow() = default;


void DrumMainWindow::createWidget()
{
    // directory Manager
    m_directoryManager = std::make_unique<Tools::DirectoryManager>();
    const auto &workingDirectory = m_directoryManager->getWorkingDirectory();

    // factories
    m_drumTabFactory = std::make_unique<Drum::DrumTabFactory>(workingDirectory);
    m_drumTabFactory->loadDirectory();
    auto& drumTab = m_drumTabFactory->getOneObject();

    m_drumTabPartFactory = std::make_unique<Drum::DrumTabPartFactory>(workingDirectory);
    m_drumTabPartFactory->loadDirectory();
    auto& drumtabPart = m_drumTabPartFactory->getOneObject();

    m_drumTabPdfPrinterConfigFactory = std::make_unique<Drum::DrumTabPdfPrinterConfigFactory>(workingDirectory);
    m_drumTabPdfPrinterConfigFactory->loadDirectory();
    auto& drumTabPdfPrinterConfig = m_drumTabPdfPrinterConfigFactory->getOneObject();

    // widgets
    m_mainWidget = new QWidget(this);
    m_tabWidget = new QWidget(m_mainWidget);
    m_creatorWidget = new QWidget(m_mainWidget);
    m_qSpacerItemCreator = new QSpacerItem(0,0,QSizePolicy::Fixed,QSizePolicy::Expanding);
    m_drumTabWidget = new DrumTabWidget(4,drumTab,m_mainWidget);
    m_drumTabPartCreatorWidget = new DrumTabPartCreatorWidget(drumtabPart,m_mainWidget);
    m_drumMainToolBar = new DrumMainToolBar(drumTab,drumTabPdfPrinterConfig,m_mainWidget);
    setCentralWidget(m_mainWidget);

    // policies
    QSizePolicy fixedPolicy(QSizePolicy::Policy::Fixed,QSizePolicy::Policy::Fixed);
    QSizePolicy ExpendingPolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Expanding);
    m_drumTabPartCreatorWidget->setSizePolicy(fixedPolicy);
    m_drumTabWidget->setSizePolicy(ExpendingPolicy);
    //no policy for m_drumMainToolBar because it is hold by the widgets inside


}

void DrumMainWindow::createLayout()
{
    m_mainLayout = new QHBoxLayout(m_mainWidget);
    m_qVBoxLayoutTab = new QVBoxLayout(m_tabWidget);
    m_qVBoxLayoutCreator = new QVBoxLayout(m_creatorWidget);
}

void DrumMainWindow::addWidgetToLayout()
{
    // creator part
    m_creatorWidget->setLayout(m_qVBoxLayoutCreator);
    m_qVBoxLayoutCreator->addWidget(m_drumTabPartCreatorWidget);
    m_qVBoxLayoutCreator->addSpacerItem(m_qSpacerItemCreator);

    // tab part
    m_tabWidget->setLayout(m_qVBoxLayoutTab);
    m_qVBoxLayoutTab->addWidget(m_drumMainToolBar);
    m_qVBoxLayoutTab->addWidget(m_drumTabWidget);

    // main window
    m_mainLayout->addWidget(m_tabWidget);
    m_mainLayout->addWidget(m_creatorWidget);
    m_mainWidget->setLayout(m_mainLayout);
}

void DrumMainWindow::connectWidget()
{

    // copy
    QObject::connect(m_drumTabWidget,
                     &DrumTabWidget::menuCopyPressed,
                     this,
                     [this](const Drum::DrumTabPart &drumTabPart)
                     {
                        m_copiedDrumTabPart = drumTabPart;
                     });

    QObject::connect(m_drumTabPartCreatorWidget,
                     &DrumTabPartWidgetBase::menuCopyPressed,
                     this,
                     [this](const Drum::DrumTabPart &drumTabPart)
                     {
                        m_copiedDrumTabPart = drumTabPart;
                     });
    // paste
    QObject::connect(m_drumTabWidget,
                     &DrumTabWidget::menuPastePressed,
                     this,
                     [this](DrumTabPartWidgetBase *sender)
                     {
                        sender->setDrumTabPart(m_copiedDrumTabPart);
                     });
    QObject::connect(m_drumTabPartCreatorWidget,
                     &DrumTabPartWidgetBase::menuPastePressed,
                     this,
                     [this](DrumTabPartWidgetBase *sender)
                     {
                        sender->setDrumTabPart(m_copiedDrumTabPart);
                     });

}
