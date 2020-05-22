#include "UI/ui_directorylineedit.h"

#include <QFileDialog>

using namespace UI;

DirectoryLineEdit::DirectoryLineEdit(QString directory,
                                     QWidget *parent) :
    QWidget(parent),
    m_chosenDirectory(directory)
{
    createWidget();
    createLayout();
    addWidgetToLayout();
    connectWidget();
}

void DirectoryLineEdit::createWidget()
{
    m_qLineEdit = new QLineEdit(m_chosenDirectory,this);
    m_qPushButton = new QPushButton(this);
    m_qPushButton->setIcon(QIcon(":/tDrum_openDirectory_tree.svg"));
}

void DirectoryLineEdit::createLayout()
{
    m_mainLayout = new QHBoxLayout(this);
}

void DirectoryLineEdit::addWidgetToLayout()
{
    m_mainLayout->addWidget(m_qLineEdit);
    m_mainLayout->addWidget(m_qPushButton);
    setLayout(m_mainLayout);
}

void DirectoryLineEdit::connectWidget()
{
    QObject::connect(m_qLineEdit,
                     &QLineEdit::textChanged,
                     this,
                     [this](const QString& directory)
                     {
                        m_chosenDirectory = directory;
                        emit directoryChanged(m_chosenDirectory);
                     });

    QObject::connect(m_qPushButton,
                     &QPushButton::clicked,
                     this,
                     [this]()
                     {
                        m_chosenDirectory = openDirectoryDialog();
                        m_qLineEdit->setText(m_chosenDirectory);
                        emit directoryChanged(m_chosenDirectory);
                     });

}

QString DirectoryLineEdit::openDirectoryDialog()
{
    QString directory = QFileDialog::getExistingDirectory(this,
                                                          "Choose directory",
                                                          m_chosenDirectory);
    if(directory == "")
        directory = m_chosenDirectory;

    return directory;
}


