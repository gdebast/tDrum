#pragma once

#include <DrumAPI/drum_drumtabpart.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QWidget>
#include <QSpacerItem>
#include <memory>



namespace DrumUI
{
    class DrumTabWidget;
    class DrumTabPartCreatorWidget;
    class DrumMainToolBar;
}

namespace Drum
{
    class DrumTabFactory;
    class DrumTabPartFactory;
    class DrumTabPdfPrinterConfigFactory;
}

namespace DrumUI
{
    class DrumMainWindow final : public QMainWindow
    {
        Q_OBJECT
    public:
        explicit DrumMainWindow(QWidget *parent = nullptr);
        ~DrumMainWindow();

        // method to be called when ending the program
        void end();

    private:

        void createWidget();
        void createLayout();
        void addWidgetToLayout();
        void connectWidget();

        DrumTabWidget                             *m_drumTabWidget{nullptr};
        DrumTabPartCreatorWidget                  *m_drumTabPartCreatorWidget{nullptr};
        DrumMainToolBar                           *m_drumMainToolBar{nullptr};

        std::unique_ptr<Drum::DrumTabFactory>                  m_drumTabFactory;
        std::unique_ptr<Drum::DrumTabPartFactory>              m_drumTabPartFactory;
        std::unique_ptr<Drum::DrumTabPdfPrinterConfigFactory>  m_drumTabPdfPrinterConfigFactory;

        Drum::DrumTabPart                          m_copiedDrumTabPart;

        QWidget                                   *m_mainWidget{nullptr};
        QWidget                                   *m_tabWidget{nullptr};
        QWidget                                   *m_creatorWidget{nullptr};
        QSpacerItem                               *m_qSpacerItemCreator{nullptr};
        QVBoxLayout                               *m_qVBoxLayoutCreator{nullptr};
        QVBoxLayout                               *m_qVBoxLayoutTab{nullptr};
        QHBoxLayout                               *m_mainLayout{nullptr};



    };
}

