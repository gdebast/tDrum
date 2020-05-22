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
                             Drum::DrumTab* modelDrumTab,
                             QWidget *parent) :
    QScrollArea(parent),

    m_drumTabModel(modelDrumTab),
    m_columnNr(columnNumber)
{

    m_lineNr = static_cast<int>(std::ceil(static_cast<float>(m_drumTabModel->getDrumTabSize())/m_columnNr));

    // create a matrix of DrumTabPartWidget
    m_drumTabWidgetInScrollingArea = new QWidget(); // main widget containing the parts
    m_mainGridLayout = new QGridLayout(); // matrix-layout of the main widget
    m_drumTabWidgetInScrollingArea->setLayout(m_mainGridLayout);
    m_mainGridLayout->setHorizontalSpacing(0);

    auto drumTabPartsImplicit = m_drumTabModel->getDrumTabParts();
    unsigned index_drumTabParts(0);

    for (int line =0; line < m_lineNr; line++)
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

    updateGridlayout();


    // include the main widget in the scrolling area
    // must be at the end after all layout updates
    setWidget(m_drumTabWidgetInScrollingArea);

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
    // find the row where the trigger is sent
    auto senderRowColumn = getDrumTabPartWidgetRowColumn(sender);
    auto senderRow = senderRowColumn.first;

    // the shift is defined by the boolean
    //  if it is true, all row below (including the sender row) is shifted by +1
    //  if it is false, all row below (excluding the sender row) is shifted by +1
    // this first shifted row is also the inserted row number
    int firstShiftedRow = senderRow + (aboveBelow==false);


    // shift
    for(auto partPositionPair : m_DrumTabPartWidget)
    {
        if(partPositionPair.second.first >= firstShiftedRow)
        {
            m_DrumTabPartWidget[partPositionPair.first] = std::make_pair(partPositionPair.second.first+1,partPositionPair.second.second);
        }
    }

    // add
    for (int column = 0; column < m_columnNr; column++)
    {
        addDrumTabPartWidget(firstShiftedRow,column);
    }
    m_lineNr++;

    updateGridlayout();



}

void DrumTabWidget::removeDrumTabRow(DrumTabPartDisplayWidget *sender)
{
    auto deletedRow = getDrumTabPartWidgetRowColumn(sender).first;

    // create a copy of the drum tab part widget with their position
    // to recreate it in the following loop
    decltype(m_DrumTabPartWidget) newDrumTabPartWidget;


    for(const auto& partPositionPair : m_DrumTabPartWidget)
    {
        auto partWigdget = partPositionPair.first;
        auto row = partPositionPair.second.first;
        auto column = partPositionPair.second.second;

        // remove the widgets of this row
        if(row == deletedRow)
        {
            m_drumTabModel->removeDrumTabPart(m_columnNr*row);
            m_mainGridLayout->removeWidget(partWigdget);
            partWigdget->hide();
            // deleting the pointer will make the app to crash...
            // this is not a problem because the drumtab-widget is the parent.

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

    m_DrumTabPartWidget = newDrumTabPartWidget;

    updateGridlayout();
    m_lineNr--;

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
    if(drumTabPartExist)
    {
        throw Drum::DrumException("Error from DrumTabWidget::addDrumTabPartWidget: "
                                  "trying to add an existing drum-tab part");
    }


    // if the row-column combination already exist,
    // we have to shift all widget
    if(rowColumnExist == false)
    {
        auto newDrumTabPartWidget = new DrumTabPartDisplayWidget(DrumTabPart,
                                                                 implicitDrawing,
                                                                 this);
        m_DrumTabPartWidget[newDrumTabPartWidget] = std::make_pair(row, column);

        // connection
        connectDrumTabPartWidget(newDrumTabPartWidget);
    }
    else
    {
        throw Drum::DrumException("Error from DrumTabWidget::addDrumTabPartWidget: "
                                  "trying to add a drum-tab part in a already used position (" +
                                  std::to_string(row) + ","+ std::to_string(column)  + ")");
    }



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
    // note : this function must be called after line nr and column nr have been updated
    for(const auto partPositionPair : m_DrumTabPartWidget)
    {
        m_mainGridLayout->addWidget(partPositionPair.first,
                                    partPositionPair.second.first,
                                    partPositionPair.second.second);
    }

    m_drumTabWidgetInScrollingArea->resize(m_columnNr*DrumTabPartDisplayWidget::getFixedWidth(),
                                           m_lineNr*DrumTabPartDisplayWidget::getFixedHeight());
}
