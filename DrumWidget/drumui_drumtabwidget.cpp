#include "DrumWidget/drumui_drumtabwidget.h"
#include "DrumWidget/drumui_drumtabpartdisplaywidget.h"
#include "DrumWidget/drumui_drumtabpartwidgetbase.h"

#include "DrumAPI/drum_drumtab.h"
#include "DrumAPI/drum_drumexception.h"

#include <cmath>

using namespace DrumUI;

// =================
// == Constructor ==
// =================

DrumTabWidget::DrumTabWidget(int columnNumber ,
                             Drum::DrumTab *modelDrumTab,
                             QWidget *parent) :
    QScrollArea(parent),

    m_drumTabModel(modelDrumTab),
    m_columnNr(columnNumber)
{

    // create a matrix of DrumTabPartWidget
    m_drumTabWidgetInScrollingArea = new QWidget(); // main widget containing the parts
    m_mainGridLayout = new QGridLayout(); // matrix-layout of the main widget
    m_drumTabWidgetInScrollingArea->setLayout(m_mainGridLayout);
    m_mainGridLayout->setHorizontalSpacing(0);
    // include the main widget in the scrolling area
    // must be at the end after all layout updates
    setWidget(m_drumTabWidgetInScrollingArea);

    // create the widget with the model
    createWidgetsWithModel();

    // fill the layout
    updateGridlayout();

}

void DrumTabWidget::setDrumTab(Drum::DrumTab *drumTabModel)
{
    m_drumTabModel = drumTabModel;
    // create the widget with the model
    createWidgetsWithModel();
    // fill the layout
    updateGridlayout();
}

// =================
// == Public SLOT ==
// =================

// change the selected drum-tab part widget from one to another
void DrumTabWidget::updateSelectedDrumTabPartWidget(DrumTabPartDisplayWidget* newSelectedDrumTabPartWidget)
{
    if(m_selectedDrumTabPartWidget == newSelectedDrumTabPartWidget)
    {
        return;
    }

    if(m_selectedDrumTabPartWidget)
    {
        m_selectedDrumTabPartWidget->toggleSelection();
    }
    m_selectedDrumTabPartWidget = newSelectedDrumTabPartWidget;
    m_selectedDrumTabPartWidget->toggleSelection();
}

// add a row of drum tab parts below (false) or above(true)
void DrumTabWidget::addDrumTabPartRow(DrumTabPartDisplayWidget *sender, bool aboveBelow)
{
    emptyGarbage();

    // find the row where the trigger is sent
    const auto [senderRow, sendColumn] = getDrumTabPartWidgetRowColumn(sender);

    // tells if this row is an incomplete row. If it is, we cannot add a row below
    std::pair<unsigned,unsigned> incompleteRow_columnAtRow;
    if (aboveBelow == false && findIncompleteRow(incompleteRow_columnAtRow)
        && incompleteRow_columnAtRow.first == static_cast<unsigned>(senderRow))
    {
        return;
    }

    // the shift is defined by the boolean
    //  if it is true, all row below (including the sender row) is shifted by +1
    //  if it is false, all row below (excluding the sender row) is shifted by +1
    // this first shifted row is also the inserted row number
    int firstShiftedRow = senderRow + (aboveBelow==false);


    // shift
    for(const auto &[drumTabPartWidget,position] : m_DrumTabPartWidget)
    {
        if(position.first >= firstShiftedRow)
        {
            m_DrumTabPartWidget[drumTabPartWidget] = std::make_pair(position.first+1,position.second);
        }
    }

    // add
    for (int column = 0; column < m_columnNr; column++)
    {
        addDrumTabPartWidget(firstShiftedRow,column);
    }
    m_rowNr++;

    updateGridlayout();



}

void DrumTabWidget::addDrumTabPartWidget(DrumTabPartDisplayWidget *sender, bool leftRight)
{
    emptyGarbage();

    // find the row where the trigger is sent
    const auto [senderRow, sendColumn] = getDrumTabPartWidgetRowColumn(sender);

    // new position computation.
    // by default, we just keep the same line with one more column if we add to the right
    // if the column goes out of range, the logic go the row above or below.
    // if the row goes out of range, we take the first position (0,0)
    int newColumn = sendColumn + (leftRight == false);
    int newRow(senderRow);
    if (newColumn < 0)
    {
        newColumn = m_columnNr-1;
        newRow = senderRow-1;
    }
    else if (newColumn >= m_columnNr)
    {
        newColumn = 0;
        newRow = senderRow+1;
    }
    if (newRow < 0)
    {
        newColumn = 0;
        newRow = 0;
    }

    // shift existing widgets
    for(const auto &[drumTabPartWidget,position] : m_DrumTabPartWidget)
    {
        // shift happens if the existing widget row is below the new row or
        // if it is at the same row and with a larger or equal column.
        if(position.first > newRow || (position.first == newRow && position.second >= newColumn))
        {
            int existingNewRow(position.first);
            int existingNewColumn(position.second+1);
            if(existingNewColumn>=m_columnNr)
            {
                existingNewRow++;
                existingNewColumn = 0;
            }
            m_DrumTabPartWidget[drumTabPartWidget] = std::make_pair(existingNewRow,existingNewColumn);
        }
    }


    addDrumTabPartWidget(newRow,newColumn);
    computeLineNr();
    updateGridlayout();

}

void DrumTabWidget::removeDrumTabRow(DrumTabPartDisplayWidget *sender)
{
    emptyGarbage();

    auto deletedRow = getDrumTabPartWidgetRowColumn(sender).first;

    // create a copy of the drum tab part widget with their position
    // to recreate it in the following loop
    decltype(m_DrumTabPartWidget) newDrumTabPartWidget;

    if (sender == m_selectedDrumTabPartWidget)
    {
        m_selectedDrumTabPartWidget = nullptr;
    }

    // assert if the model exist
    Drum::DrumException::drumAssert(m_drumTabModel,
                                    "Error from DrumTabWidget::removeDrumTabRow: "
                                    "trying to remove a row of drum tab parts}."
                                    "It is impossible since the model is nullptr.");

    for(const auto& partPositionPair : m_DrumTabPartWidget)
    {
        auto partWigdget = partPositionPair.first;
        auto row = partPositionPair.second.first;
        auto column = partPositionPair.second.second;

        // remove the widgets of this row
        if(row == deletedRow)
        {
            m_drumTabModel->removeDrumTabPart(m_columnNr*row);
            partWigdget->hide();
            // do not delete the widget otherwise the widget were it is triggered is a bad ptr

            // we rather put it in the garbage
            m_removedDrumTabPartDisplayWidgets.push_back(partWigdget);
        }
        // shift the widget of all row below
        else if(row > deletedRow)
        {
            newDrumTabPartWidget[partWigdget] = std::make_pair(row-1,column);
        }
        // copy all widget of all row above
        else
        {
            newDrumTabPartWidget[partWigdget] = std::make_pair(row,column);
        }

    }

    m_DrumTabPartWidget = std::move(newDrumTabPartWidget);

    m_rowNr--;
    updateGridlayout();


}

void DrumTabWidget::removeDrumTabPartWidget(DrumTabPartDisplayWidget *sender)
{
    emptyGarbage();

    // find the row where the trigger is sent
    const auto [senderRow, senderColumn] = getDrumTabPartWidgetRowColumn(sender);

    // create a copy of the drum tab part widget with their position
    // to recreate it in the following loop
    decltype(m_DrumTabPartWidget) newDrumTabPartWidget;

    if (sender == m_selectedDrumTabPartWidget)
    {
        m_selectedDrumTabPartWidget = nullptr;
    }

    // assert if the model exist
    Drum::DrumException::drumAssert(m_drumTabModel,
                                    "Error from DrumTabWidget::removeDrumTabPartWidget: "
                                    "trying to remove a drum tab part."
                                    "It is impossible since the model is nullptr.");

    for(const auto& [drumTabPartWidget,position] : m_DrumTabPartWidget)
    {
        auto row = position.first;
        auto column = position.second;

        // remove the widget at this position
        if(row == senderRow && column == senderColumn)
        {

            m_drumTabModel->removeDrumTabPart(m_columnNr*row+column);
            drumTabPartWidget->hide();
            // do not delete the widget otherwise the widget were it is triggered is a bad ptr

            // we rather put it in the garbage
            m_removedDrumTabPartDisplayWidgets.push_back(drumTabPartWidget);


        }
        // shift the existing widget
        else if(row > senderRow || (row == senderRow && column > senderColumn))
        {
            // by default we shift by one column to the left
            // but if the column goes negative, it is moved to the lower line
            // and if the line goes negative, it is move to the first position
            int newRow(row);
            int newColumn(column-1);
            if (newColumn < 0)
            {
                newColumn = m_columnNr-1;
                newRow--;
            }
            if (newRow < 0)
            {
                newRow = 0;
                newColumn = 0;
            }

            newDrumTabPartWidget[drumTabPartWidget] = std::make_pair(newRow,newColumn);
        }
        // copy all widgets if they are not affected
        else
        {
            newDrumTabPartWidget[drumTabPartWidget] = std::make_pair(row,column);
        }

    }

    m_DrumTabPartWidget = std::move(newDrumTabPartWidget);

    computeLineNr();
    updateGridlayout();


}

// =======================
// == Private functions ==
// =======================

// function creating all connection for a tab-part widget
void DrumTabWidget::connectDrumTabPartWidget(DrumTabPartDisplayWidget *widget)
{
    // selection connection
    QObject::connect(widget,
                     &DrumTabPartDisplayWidget::selected,
                     this,
                     [this](DrumTabPartDisplayWidget* sender)
                     {
                        updateSelectedDrumTabPartWidget(sender);
                     });

    // copy connection
    QObject::connect(widget,
                     &DrumTabPartWidgetBase::menuCopyPressed,
                     this,
                     [this](const Drum::DrumTabPart& drumTabPart)
                     {
                        emit menuCopyPressed(drumTabPart);
                     });

    // paste connection
    QObject::connect(widget,&DrumTabPartWidgetBase::menuPastePressed,
                     this,
                     [this](DrumTabPartWidgetBase* widget)
                     {
                        emit menuPastePressed(widget);
                     });


    // add above below connection
    QObject::connect(widget,
                     &DrumTabPartDisplayWidget::menuAddTabLinePressed,
                     this,
                     [this](DrumTabPartDisplayWidget* sender,bool aboveBelow)
                     {
                        addDrumTabPartRow(sender,aboveBelow);
                     });

    // remove connection
    QObject::connect(widget,
                     &DrumTabPartDisplayWidget::menuRemoveTabLinePressed,
                     this,
                     [this](DrumTabPartDisplayWidget* sender)
                     {
                        removeDrumTabRow(sender);
                     });

    // implicit/explicit setting connection
    QObject::connect(widget,
                     &DrumTabPartDisplayWidget::explicitImplicitToggled,
                     this,
                     [this](DrumTabPartDisplayWidget* sender)
                     {
                        sender->toggleExplicitImplicit();
                        m_drumTabModel->setImplicitExplicit(sender->getDrumTabPart(),sender->isImplicitDrawn());
                     });


    // add part left/right connection
    QObject::connect(widget,
                     &DrumTabPartDisplayWidget::menuAddTabPartPressed,
                     this,
                     [this](DrumTabPartDisplayWidget* sender,bool rightLeft)
                     {
                        addDrumTabPartWidget(sender,rightLeft);
                     });

    // remove part connection
    QObject::connect(widget,
                     &DrumTabPartDisplayWidget::menuRemoveTabPartPressed,
                     this,
                     [this](DrumTabPartDisplayWidget* sender)
                     {
                        removeDrumTabPartWidget(sender);
                     });
}

// function adding a tab-part widget to this tab widget
void DrumTabWidget::addDrumTabPartWidget(int row,
                                         int column,
                                         Drum::DrumTabPart *newPart,
                                         bool implicitDrawing)
{

    Drum::DrumTabPart* DrumTabPart{nullptr};
    if(newPart)
    {
        DrumTabPart = newPart;
    }
    // create one
    else
    {
        // assert if the model exist
        Drum::DrumException::drumAssert(m_drumTabModel,
                                        "Error from DrumTabWidget::addDrumTabPartWidget: "
                                        "trying to create a drum tab part for row-column {},{}."
                                        "It is impossible since the model is nullptr.",
                                        row,column);

        DrumTabPart = m_drumTabModel->addDrumTabPart(column+m_columnNr*row); // add empty drum tab part
    }

    // check that the row-column does not already exist
    bool rowColumnExist(false);   // the row should not exist yet
    bool drumTabPartExist(false); // the drumtabPart that we are trying to add should not exist
    auto it = m_DrumTabPartWidget.begin();
    while (it != m_DrumTabPartWidget.end() && rowColumnExist == false && drumTabPartExist == false)
    {
        rowColumnExist = it->second.first == row && it->second.second == column;
        drumTabPartExist = it->first->getDrumTabPart() == DrumTabPart;
        it++;
    }

    // assert if the drum tab exist
    Drum::DrumException::drumAssert(drumTabPartExist == false,
                                    "Error from DrumTabWidget::addDrumTabPartWidget: "
                                    "trying to add an existing drum-tab part at position {},{}.",
                                    row,column);

    // assert if the column-row is already taken
    Drum::DrumException::drumAssert(rowColumnExist == false,
                                    "Error from DrumTabWidget::addDrumTabPartWidget: "
                                    "trying to add a drum-tab part in a already used position : {},{}.",
                                    row,column);

    // add a new drum tab part widget
    auto newDrumTabPartWidget = new DrumTabPartDisplayWidget(DrumTabPart,
                                                             implicitDrawing,
                                                             this);
    m_DrumTabPartWidget[newDrumTabPartWidget] = std::make_pair(row, column);

    // connection
    connectDrumTabPartWidget(newDrumTabPartWidget);





}

std::pair<int,int> DrumTabWidget::getDrumTabPartWidgetRowColumn(DrumTabPartDisplayWidget *widget) const
{
    auto it = m_DrumTabPartWidget.find(widget);

    if(it == m_DrumTabPartWidget.end())
    {
        std::string nullptrMessage("");
        if(widget == nullptr)
        {
            nullptrMessage= " The widget is nullptr.";
        }

        throw Drum::DrumException("Error from DrumTabWidget::getDrumTabPartWidgetRowColumn: "
                                  "did not find the widget." + nullptrMessage);
    }
    return it->second;
}

void DrumTabWidget::updateGridlayout()
{
    // empty the layouts
    QLayoutItem *item;
    while ((item = m_mainGridLayout->takeAt(0)) != nullptr)
    {
        delete item;
    }
    m_gridSpacerForIncompleteRow = nullptr; // deleted with all items

    // note : this function must be called after line nr and column nr have been updated
    for(const auto &[drumTabPartWidget,rowColumn] : m_DrumTabPartWidget)
    {

        // assert if the column-row is already taken
        Drum::DrumException::drumAssert(rowColumn.first < m_rowNr && rowColumn.second < m_columnNr,
                                        "Error from DrumTabWidget::updateGridlayout: "
                                        "when lopping over drum tab parts, one of the parts "
                                        "is at position ({},{}) while the maximum number of "
                                        "rows and columns are {} and {}",
                                        rowColumn.first,rowColumn.second,
                                        m_rowNr,m_columnNr);

        m_mainGridLayout->addWidget(drumTabPartWidget,
                                    rowColumn.first,
                                    rowColumn.second);
    }

    // add a spacer at the end if the last row is incomplete
    std::pair<unsigned,unsigned> incompleteRow_columnAtRow;
    if (findIncompleteRow(incompleteRow_columnAtRow))
    {
        m_gridSpacerForIncompleteRow = new QSpacerItem(0,0, QSizePolicy::Expanding);
        m_mainGridLayout->addItem(m_gridSpacerForIncompleteRow,
                                  incompleteRow_columnAtRow.first,
                                  incompleteRow_columnAtRow.second,
                                  1,m_columnNr - incompleteRow_columnAtRow.second);
    }



    m_drumTabWidgetInScrollingArea->resize(m_columnNr*DrumTabPartDisplayWidget::getFixedWidth(),
                                           m_rowNr*DrumTabPartDisplayWidget::getFixedHeight());
}

void DrumTabWidget::createWidgetsWithModel()
{
    // empty the widget if they exist
    for (const auto &[widget,rowColumn] : m_DrumTabPartWidget)
    {
        delete widget;
    }
    m_selectedDrumTabPartWidget = nullptr;
    m_DrumTabPartWidget.clear();

    // do not continue if the model is nullptr
    if (m_drumTabModel == nullptr)
        return;

    computeLineNr();

    // fill the widget
    auto drumTabPartsImplicit = m_drumTabModel->getDrumTabParts();
    unsigned index_drumTabParts(0);
    for (int line =0; line < m_rowNr; line++)
    {
        for (int column=0; column < m_columnNr; column++)
        {

            Drum::DrumTabPart* DrumTabPart{nullptr};
            bool implicitDrawing(false);
            if(index_drumTabParts < drumTabPartsImplicit.size())
            {
                DrumTabPart = drumTabPartsImplicit[index_drumTabParts].first;
                implicitDrawing = drumTabPartsImplicit[index_drumTabParts].second;
            }

            addDrumTabPartWidget(line, column, DrumTabPart,implicitDrawing);
            index_drumTabParts++;

        }
    }
}

void DrumTabWidget::computeLineNr()
{
    m_rowNr = static_cast<int>(std::ceil(static_cast<float>(m_drumTabModel->getDrumTabSize())/m_columnNr));
}

void DrumTabWidget::emptyGarbage()
{
    for (auto *widget : m_removedDrumTabPartDisplayWidgets)
    {
        delete widget;
    }
    m_removedDrumTabPartDisplayWidgets.clear();
}

bool DrumTabWidget::findIncompleteRow(std::pair<unsigned, unsigned> &incompleteRow_ColumnAtRow)
{
    //find the rows with incomplete number of columns
    std::map<int,int> rowColumnNbr;
    for(const auto &[drumTabPartWidget,rowColumn] : m_DrumTabPartWidget)
    {
        rowColumnNbr[rowColumn.first]++;
    }
    std::vector<int> incompleteRows;
    for (const auto &[row,nrbOfColumn] : rowColumnNbr)
    {
        if (nrbOfColumn != m_columnNr)
            incompleteRows.push_back(row);
    }
    Drum::DrumException::drumAssert(incompleteRows.size() <= 1,
                                    "Error from DrumTabWidget::findIncompleteRow: "
                                    "there are more then one incomplete row.");

    if (incompleteRows.size() == 1)
    {
        incompleteRow_ColumnAtRow.first = incompleteRows[0];
        incompleteRow_ColumnAtRow.second = rowColumnNbr[incompleteRows[0]];
    }

    return incompleteRows.size() == 1;
}
