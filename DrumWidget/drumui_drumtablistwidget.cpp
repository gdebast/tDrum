#include "DrumWidget/drumui_drumtablistwidget.h"

#include "DrumAPI/drum_drumtabfactory.h"
#include "DrumAPI/drum_drumtab.h"
#include "Tools/tools_exception.h"
#include "DrumWidget/drumui_drumtabpartwidgetbase.h"

#include <QHeaderView>

using namespace DrumUI;

const std::vector<std::pair<bool,std::string>> DrumTabListWidget::s_headers =
{
    std::make_pair(false,"Button"),
    std::make_pair(true,"Author"),
    std::make_pair(true,"Title")
};

DrumTabListWidget::DrumTabListWidget(Drum::DrumTabFactory& drumTabFactory,
                                     QWidget *parent) :
    QWidget(parent),
    m_drumTabFactory(drumTabFactory)
{
    m_mainLayout = new QVBoxLayout(this);
    setLayout(m_mainLayout);

    // create the empty table
    m_tableWidget = new QTableWidget(this);
    m_mainLayout->addWidget(m_tableWidget);
    m_tableWidget->setRowCount(1);
    m_tableWidget->setColumnCount(s_headers.size());
    m_tableWidget->verticalHeader()->setVisible(false);
    m_tableWidget->setShowGrid(false);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    m_addButton = new QPushButton(m_tableWidget);
    m_addButton->setIcon(QIcon(":/tDrum_add.svg"));
    m_addButton->setStyleSheet("border: none;");
    m_tableWidget->setCellWidget(0,0,m_addButton);
    QStringList tableHeader;
    for (const auto &[shown, header] : s_headers)
    {
        if(shown)
        {
            tableHeader << header.c_str();
        }
        else
        {
            tableHeader << "";
        }
    }
    m_tableWidget->setHorizontalHeaderLabels(tableHeader);

    // add a row per drum tab in the factory
    for(auto* drumTab : m_drumTabFactory.getObjects())
    {
        addRow(drumTab);
    }

    m_tableWidget->resizeColumnsToContents();

    // connect the widgets
    connectStaticWidget();

}

void DrumTabListWidget::drumTabTitleAuthorChanged(Drum::DrumTab &changedDrumTab)
{
    auto it = m_drumTabButtonMap.find(&changedDrumTab);
    // assert if the list does not contain at least the add button
    Tools::Exception::Assert(it != m_drumTabButtonMap.end(),
                             "Error from DrumTabListWidget::deleteRow: "
                             "cannot find the pointer.");


    unsigned row = m_tableWidget->row(it->second.first);

    auto* titleItem = m_tableWidget->item(row,getHeaderColumn("Title"));
    titleItem->setText(changedDrumTab.getTitle().c_str());

    auto* authorItem = m_tableWidget->item(row,getHeaderColumn("Author"));
    authorItem->setText(changedDrumTab.getAuthor().c_str());

}

void DrumTabListWidget::addRow(Drum::DrumTab *drumTab)
{
    Drum::DrumTab *drumTabInRow;
    if (drumTab)
    {
        drumTabInRow = drumTab;
    }
    else
    {
        drumTabInRow = &m_drumTabFactory.createDefaultObject();
    }

    // assert if the drum tab is already a row
    Tools::Exception::Assert(m_drumTabButtonMap.find(drumTabInRow) == m_drumTabButtonMap.end(),
                            "Error from DrumTabListWidget::addRow: "
                            "trying to add an existing drum-tab part");


    // assert if the list does not contain at least the add button
    Tools::Exception::Assert(m_tableWidget->rowCount() > 0,
                             "Error from DrumTabListWidget::addRow: "
                             "missing add button.");

    // add a row
    unsigned rowNumber(m_tableWidget->rowCount()-1);
    auto* deleteButton = new QPushButton(m_tableWidget);
    deleteButton->setIcon(QIcon(":/tDrum_delete.svg"));
    deleteButton->setStyleSheet("border: none;");

    m_tableWidget->insertRow(rowNumber);
    unsigned column(0);
    for (const auto &[show,header]: s_headers)
    {
        if(header == "Author")
        {
            auto *item = new QTableWidgetItem(drumTabInRow->getAuthor().c_str());
            m_drumTabButtonMap[drumTabInRow] = std::make_pair(item,deleteButton);
            m_tableWidget->setItem(rowNumber,column,item);
        }
        else if(header == "Title")
        {
            auto *item = new QTableWidgetItem(drumTabInRow->getTitle().c_str());
            m_tableWidget->setItem(rowNumber,column,item);
        }
        else
        {
            m_tableWidget->setCellWidget(rowNumber,column,deleteButton);
        }
        column++;
    }

    // connect the delete button
    QObject::connect(deleteButton,&QPushButton::clicked,
                     this,[drumTabInRow,this](){deleteRow(*drumTabInRow);});




}

void DrumTabListWidget::deleteRow(Drum::DrumTab &drumTab)
{
    auto it = m_drumTabButtonMap.find(&drumTab);
    // assert if the list does not contain at least the add button
    Tools::Exception::Assert(it != m_drumTabButtonMap.end(),
                             "Error from DrumTabListWidget::deleteRow: "
                             "cannot find the pointer.");

    unsigned deletedRow(m_tableWidget->row(it->second.first));
    m_tableWidget->removeRow(deletedRow);
    delete it->second.second;
    m_drumTabButtonMap.erase(it);
    m_drumTabFactory.deleteObject(drumTab);

    // assert that the number of object in the factory and the widget it the same
    unsigned factorySize = m_drumTabFactory.getNumberOfInstances();
    unsigned widgetSize = m_drumTabButtonMap.size();
    Tools::Exception::Assert(factorySize == widgetSize,
                             "Error from DrumTabListWidget::deleteRow: "
                             "factory and widget do not haver the same size."
                             "Measured {} and {}",factorySize,widgetSize);

    // assert that the number of row is positive because at least the add button exists
    int numberOfRow = m_tableWidget->rowCount();
    Tools::Exception::Assert(numberOfRow > 0,
                             "Error from DrumTabListWidget::deleteRow: "
                             "the number of remaining rows in not positive."
                             "Measured {}",numberOfRow);

    // set the focus to the closest upper item (deletedRow-1) if possible
    unsigned newSelectedRow(deletedRow > 0 ? deletedRow-1 : 0);
    auto *newSelectedDrumTab = getDrumTabAtRow(newSelectedRow);
    m_tableWidget->selectRow(newSelectedRow); //note: QTableWidget::selectRow does trigger QTableWidget::cellClicked
    emit drumTabSelected(newSelectedDrumTab);

}

int DrumTabListWidget::getHeaderColumn(const std::string &headerName) const
{
    unsigned column(0);
    for(const auto &[shown, header] : s_headers)
    {
        if (header == headerName)
            return column;
        column++;
    }
    return -1;
}

Drum::DrumTab *DrumTabListWidget::getDrumTabAtRow(int row) const
{
    for(const auto &[drumTab, itemButtonPair] : m_drumTabButtonMap)
    {
        if(row == m_tableWidget->row(itemButtonPair.first))
        {
            return drumTab;
        }
    }
    return nullptr;
}

void DrumTabListWidget::connectStaticWidget()
{
    // connect the unique add button
    QObject::connect(m_addButton,
                     &QPushButton::clicked,
                     this,
                     [this]()
                          {
                            addRow();
                          });

    // connect selected row
    QObject::connect(m_tableWidget,
                     &QTableWidget::cellClicked,
                     this,
                     [this](int row, int)
                          {
                            auto *drumTab = getDrumTabAtRow(row);
                            emit drumTabSelected(drumTab);
                          });
}


