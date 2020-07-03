#pragma once

#include <map>
#include <memory>
#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

namespace Drum
{
    class DrumTabFactory;
    class DrumTab;
}

namespace DrumUI
{
    class DrumTabListWidget : public QWidget
    {
        Q_OBJECT
        public:
            explicit DrumTabListWidget(Drum::DrumTabFactory &drumTabFactory,
                                       QWidget *parent = nullptr);

        signals:
            // signal indicating that a new DrumTab is selected in this list
            void drumTabSelected(Drum::DrumTab*);

        public slots:
            // slot to change the Author and Title in the table
            void drumTabTitleAuthorChanged(Drum::DrumTab& changedDrumTab);

        private:


            void addRow(Drum::DrumTab* drumTab = nullptr);
            void deleteRow(Drum::DrumTab& drumTab);
            int getHeaderColumn(const std::string& headerName) const;
            Drum::DrumTab *getDrumTabAtRow(int row) const;
            void connectStaticWidget();

            std::map<Drum::DrumTab*,std::pair<QTableWidgetItem*,QPushButton*>>  m_drumTabButtonMap;
            Drum::DrumTabFactory                                               &m_drumTabFactory;

            QVBoxLayout  *m_mainLayout;
            QTableWidget *m_tableWidget;
            QPushButton  *m_addButton;

            const static std::vector<std::pair<bool,std::string>> s_headers; // the boolean tells if the header is shown

    };
}




