#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

namespace  Tools
{
    class DirectoryManager;
}

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
            explicit DirectoryLineEdit(const Tools::DirectoryManager& directoryManager,
                                       QString directory = "",
                                       QWidget *parent = nullptr);

            // tells if the chosen directory is ok
            bool isDirectoryOk() const;

        signals:
            void directoryChanged(QString directory);

        private:
            void createWidget();
            void createLayout();
            void addWidgetToLayout();
            void connectWidget();
            void setChosenDirectory(const QString& chosenDirectory);
            QString openDirectoryDialog();

            QString m_chosenDirectory;

            QLineEdit*   m_qLineEdit{nullptr};
            QPushButton* m_qPushButton{nullptr};
            QHBoxLayout* m_mainLayout{nullptr};

            bool m_isDirectoryOk{false};

            const Tools::DirectoryManager &m_directoryManager;

    };
}



