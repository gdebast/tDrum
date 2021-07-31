#include "UI/ui_directorylineedit.h"

#include <Tools/tools_directorymanager.h>

#include <QFileDialog>


using namespace UI;

DirectoryLineEdit::DirectoryLineEdit(const Tools::DirectoryManager& directoryManager,
                                     QString directory,
                                     QWidget *parent) :
    QWidget(parent),
    m_chosenDirectory(directory),
    m_directoryManager(directoryManager)
{
    setChosenDirectory(directory);
    createWidget();
    createLayout();
    addWidgetToLayout();
    connectWidget();
}

bool DirectoryLineEdit::isDirectoryOk() const
{
    return m_isDirectoryOk;
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
                        setChosenDirectory(directory);
                        emit directoryChanged(m_chosenDirectory);
                     });

    QObject::connect(m_qPushButton,
                     &QPushButton::clicked,
                     this,
                     [this]()
                     {
                        setChosenDirectory(openDirectoryDialog());
                        m_qLineEdit->setText(m_chosenDirectory);
                        emit directoryChanged(m_chosenDirectory);
                     });

}

void DirectoryLineEdit::setChosenDirectory(const QString& chosenDirectory)
{
    m_chosenDirectory = chosenDirectory;
    m_isDirectoryOk = m_directoryManager.isExistingDirectory(chosenDirectory.toStdString());
    QPalette palette;

    if(m_isDirectoryOk)
    {
        setStyleSheet("QLineEdit{background : transparent;}");
    }
    else
    {
        setStyleSheet("QLineEdit{background : red;}");
    }
    setPalette(palette);
}

QString DirectoryLineEdit::openDirectoryDialog()
{
    QString directory = QFileDialog::getExistingDirectory(this,
                                                          "Choose directory",
                                                          m_chosenDirectory);
    if(directory == "")
        directory = m_chosenDirectory;
    if(*directory.rbegin() != '/')
        directory+='/';

    return directory;
}


