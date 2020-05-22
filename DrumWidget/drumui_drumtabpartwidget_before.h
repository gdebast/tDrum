#ifndef DRUMTABPARTWIDGET_H
#define DRUMTABPARTWIDGET_H

#include "drum_drumkit.h"

#include <QWidget>
#include <QMenu>

namespace Drum {
    class DrumTabPart;
}

namespace DrumUI{

    /*
     * Widget displaying a DrumTabPart without any possibility of editing it
     * It is responsible for handling the User interface for DrumTabPart.
     */

    class DrumTabPartWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit DrumTabPartWidget(Drum::DrumTabPart* drumTabPartModel, QWidget *parent = nullptr);
        void toggleSelection();
        void pasteModel(const Drum::DrumTabPart& newDrumTabPartModel);
        void clearModel();
        Drum::DrumTabPart* getModel() const {return m_drumTabPartModel;};

        static int getFixedHeight();
        static int getFixedWidth();


    protected:
        // -- from QWidget
        virtual void paintEvent(QPaintEvent* e) override;
        virtual QSize sizeHint() const override;
        virtual void enterEvent(QEvent *event) override;
        virtual void leaveEvent(QEvent *event) override;
        virtual void mousePressEvent(QMouseEvent *event) override;

    signals:
        void selected(DrumTabPartWidget* selectedWidget);
        void menuClearPressed(DrumTabPartWidget* sender);
        void menuCopyPressed(DrumTabPartWidget* sender);
        void menuPastePressed(DrumTabPartWidget* sender);
        void menuAddTabLinePressed(DrumTabPartWidget* sender,bool aboveBelow); //Above = true, Below=false
        void menuRemoveTabLinePressed(DrumTabPartWidget* sender);

    public slots:

    private:

        // geometry of all drum tab parts
        // gives the position of each lines and some constants shaping the tab part
        struct Geometry
        {
            // constant
            static const int HLINETHICHNESSRATIO{100};
            static const int VLINETHICHNESSRATIO{10};
            static const int VOIDSRATIO{4};
            static const int VOIDSENDLINERATIO{3};
            static const int LINENUMBER{6};
            static const int BASEHEIGHT{120};
            static const int BASEWIDTH{220};
            static const int WIDTHRXRATIO{20};

            // member
            int height;
            int width;

            // calculated:
            //  lines y coordinates
            int BassDrumLine, FloorTomLine, SnareTomLine, HighTomLine, HiHatLine, CrashLine;
            //  line spacing
            int LineSpacing;
            //  line thickness
            int VLineThickness;
            int HLineThickness;
            int HCrocheLineThickness;
            //  drum kit size
            int DrumKitXsize;
            //  end of the drum kit vertical lines
            int VLineUpperEnd;
            int VLineBottomEnd;

            Geometry(int width, int height);
        };

        // private drawing functions
        // -------------------------
        void drawTabLines();
        void drawSelectedHovered();
        void drawDrumKits();
        void drawCymbal(int xCenter, int line);
        void drawCircledCymbal(int xCenter, int line);
        void drawUnderscoredCircledCymbal(int xCenter, int line);
        void drawUnderscoredCymbal(int xCenter, int line);
        void drawTom(int xCenter, int line);
        void drawBell(int xCenter, int line);
        void drawUnderscoredBell(int xCenter, int line);
        void drawBottomLine(int xCenter, int line, bool shift = true);
        void drawUpperLine(int xCenter, int line, bool shift = true);
        void drawDrumKit(int xCenter,Drum::DrumKit drumKit);
        void drawDrumLines();


        // model
        Drum::DrumTabPart *m_drumTabPartModel{nullptr};

        // UI
        bool     m_selected{false};
        QMenu   *m_rightClickMenu{nullptr};
        QAction *m_rightClickMenu_copy{nullptr};
        QAction *m_rightClickMenu_paste{nullptr};
        QAction *m_rightClickMenu_clear{nullptr};
        QAction *m_rightClickMenu_AddTabUnder{nullptr};
        QAction *m_rightClickMenu_AddTabOver{nullptr};
        QAction *m_rightClickMenu_RemoveTabLine{nullptr};

    };

}

#endif // DRUMTABPARTWIDGET_H
