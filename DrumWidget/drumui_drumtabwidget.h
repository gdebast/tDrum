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

        // functions
        void addDrumTabPartRow(DrumTabPartDisplayWidget* sender,bool aboveBelow);
        void removeDrumTabRow(DrumTabPartDisplayWidget* sender);
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


        void removeDrumTabPartWidget(DrumTabPartDisplayWidget* widget);
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

        // model
        Drum::DrumTab* m_drumTabModel;

        // UI
        int                                              m_columnNr{0};
        int                                              m_lineNr{0};
        QGridLayout                                     *m_mainGridLayout{nullptr};
        DrumTabPartDisplayWidget                        *m_selectedDrumTabPartWidget{nullptr};
        std::map<DrumTabPartDisplayWidget*,std::pair<int,int>>  m_DrumTabPartWidget;
        QWidget                                         *m_drumTabWidgetInScrollingArea{nullptr};



    };
}
#endif // DRUMUI_DRUMTABWIDGET_H
