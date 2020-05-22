#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

namespace UI
{
    class DirectoryLineEdit : public QWidget
    {
        /*
         * Widget used to get Directory. It contains a QLineEdit and a button to open
         * a dialog allowing to choose a directory.
         */

        Q_OBJECT
        public:
            explicit DirectoryLineEdit(QString directory = "", QWidget *parent = nullptr);

        signals:
            void directoryChanged(QString directory);

        private:
            void createWidget();
            void createLayout();
            void addWidgetToLayout();
            void connectWidget();
            QString openDirectoryDialog();

            QString m_chosenDirectory;

            QLineEdit*   m_qLineEdit{nullptr};
            QPushButton* m_qPushButton{nullptr};
            QHBoxLayout* m_mainLayout{nullptr};

    };
}



