#ifndef DRUMUI_DRUMTABWIDGET_H
#define DRUMUI_DRUMTABWIDGET_H

#include "DrumAPI/drum_drumtabpart.h"

#include <QScrollArea>
#include <QGridLayout>

#include <vector>
#include <map>

namespace Drum {
    class DrumTab;
}

namespace DrumUI {
    class DrumTabPartDisplayWidget;
    class DrumTabPartWidgetBase;
}

namespace DrumUI {

    /*
     * Widget displaying a DrumTab
     */

    class DrumTabWidget : public QScrollArea
    {
        Q_OBJECT
    public:
        explicit DrumTabWidget(int columnNumber,
                               Drum::DrumTab *drumTabModel, // can be nullptr which means that the widget is empty
                               QWidget *parent = nullptr);

        /*
         * setter for the model in the widget.
         * the given widget can be nullptr which means that
         * the widget is emptied.
         */
        void setDrumTab(Drum::DrumTab* drumTabModel);

    signals:
        void menuCopyPressed(const Drum::DrumTabPart& copiedDrumTabPart);
        void menuPastePressed(DrumTabPartWidgetBase* sender);

    private:

        // functions used in slots
        void addDrumTabPartRow(DrumTabPartDisplayWidget* sender,bool aboveBelow /*above=true,below=false*/);
        void addDrumTabPartWidget(DrumTabPartDisplayWidget* sender,bool leftRight);
        void removeDrumTabRow(DrumTabPartDisplayWidget* sender);
        void removeDrumTabPartWidget(DrumTabPartDisplayWidget* sender);
        void updateSelectedDrumTabPartWidget(DrumTabPartDisplayWidget* newSelectedDrumTabPartWidget);

        void connectDrumTabPartWidget(DrumTabPartDisplayWidget* widget);

        /*
         * method to add a DrumTabPart to the DrumTab widget at this row and column.
         * If the new DrumTabPart is nullptr, then this widget will ask to the model
         * to add a new DrumTabPart which is drawn explicitly (last argument).
         */
        void addDrumTabPartWidget(int row,
                                  int colum,
                                  Drum::DrumTabPart* newPart = nullptr,
                                  bool implicitDrawing = false);


        std::pair<int,int> getDrumTabPartWidgetRowColumn(DrumTabPartDisplayWidget* widget) const; // (row,column)

        /*
         * method filling the layout with the drum tab part widgets.
         */
        void updateGridlayout();

        /*
         * method responsible for creating Drum tab part widget with the model DrumTab.
         * m_DrumTabPartWidget is thus filled and also pre-emptied.
         */
        void createWidgetsWithModel();

        /*
         * method which simply set m_lineNr based on the number of column in m_columnNr.
         */
        void computeLineNr();

        /*
         * method to empty the garbage. The garbage consist of Widgets which
         * could not have been deleted at a right moment and thus should have been deleted later.
         */
        void emptyGarbage();

        /*
         * method returning the incomplete row (first in the reference pair)
         * and the number of column at this row (second in the reference pair).
         * The bool tells if such a row was found.
         */
        bool findIncompleteRow(std::pair<unsigned,unsigned> &incompleteRow_ColumnAtRow);

        typedef std::map<DrumTabPartDisplayWidget*,std::pair<int,int>> DrumTabPartWidgetPositionMap;

        // model
        Drum::DrumTab* m_drumTabModel;

        // UI
        int                            m_columnNr{0};
        int                            m_rowNr{0};
        QGridLayout                   *m_mainGridLayout{nullptr};
        DrumTabPartDisplayWidget      *m_selectedDrumTabPartWidget{nullptr};
        DrumTabPartWidgetPositionMap   m_DrumTabPartWidget;
        QWidget                       *m_drumTabWidgetInScrollingArea{nullptr};
        QSpacerItem                   *m_gridSpacerForIncompleteRow{nullptr};

        // garbage collections
        std::vector<DrumTabPartDisplayWidget*>               m_removedDrumTabPartDisplayWidgets;




    };
}
#endif // DRUMUI_DRUMTABWIDGET_H
