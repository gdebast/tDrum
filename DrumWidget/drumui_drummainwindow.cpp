#include "DrumWidget/drumui_drummainwindow.h"

#include "DrumAPI/drum_drumtabfactory.h"
#include "DrumAPI/drum_drumtabpartfactory.h"
#include "DrumAPI/drum_drumtabpdfprinterconfigfactory.h"
#include "DrumWidget/drumui_drumtabwidget.h"
#include "DrumWidget/drumui_drumtabpartcreatorwidget.h"
#include "DrumWidget/drumui_drumtablistwidget.h"
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
    m_drumTabWidget = new DrumTabWidget(4,&drumTab,m_tabWidget);
    m_drumMainToolBar = new DrumMainToolBar(&drumTab,drumTabPdfPrinterConfig,m_tabWidget);
    m_drumTabPartCreatorWidget = new DrumTabPartCreatorWidget(drumtabPart,m_creatorWidget);
    m_drumTabListWidget = new DrumTabListWidget(*m_drumTabFactory,m_creatorWidget);
    setCentralWidget(m_mainWidget);

    // policies
    QSizePolicy fixedPolicy(QSizePolicy::Policy::Fixed,QSizePolicy::Policy::Fixed);
    QSizePolicy ExpendingPolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Expanding);
    m_drumTabPartCreatorWidget->setSizePolicy(fixedPolicy);
    m_drumTabListWidget->setSizePolicy(ExpendingPolicy);
    m_drumTabWidget->setSizePolicy(ExpendingPolicy);
    //no policy for m_drumMainToolBar because it is hold by the widgets inside

    // size
    m_creatorWidget->setMaximumWidth(m_drumTabPartCreatorWidget->getWidth());


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
    m_qVBoxLayoutCreator->addWidget(m_drumTabListWidget);

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

    // connect the list Widget
    QObject::connect(m_drumMainToolBar,
                     &DrumMainToolBar::drumTabChanged,
                     m_drumTabListWidget,
                     &DrumTabListWidget::drumTabTitleAuthorChanged);

    QObject::connect(m_drumTabListWidget,
                     &DrumTabListWidget::drumTabSelected,
                     m_drumTabWidget,
                     &DrumTabWidget::setDrumTab);

    QObject::connect(m_drumTabListWidget,
                     &DrumTabListWidget::drumTabSelected,
                     m_drumMainToolBar,
                     &DrumMainToolBar::setDrumTab);

}
