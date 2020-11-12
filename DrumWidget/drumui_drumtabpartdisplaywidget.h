#ifndef DRUMTABPARTWIDGET_H
#define DRUMTABPARTWIDGET_H

#include "DrumWidget/drumui_drumtabpartwidgetbase.h"


namespace Drum {
    class DrumTabPart;
}

namespace DrumUI{


    /*
     * Widget displaying a DrumTabPart in a drum-tab widget
     * this part is displayed with others in the drum-tab widget
     */

    class DrumTabPartDisplayWidget final : public DrumTabPartWidgetBase
    {
        Q_OBJECT
    public:
        explicit DrumTabPartDisplayWidget(Drum::DrumTabPart* drumTabPartModel,
                                          bool implicitDrawing,
                                          double zoomFactor,
                                          QWidget *parent = nullptr);

        DrumTabPartDisplayWidget(const DrumTabPartDisplayWidget&) = delete;
        DrumTabPartDisplayWidget(DrumTabPartDisplayWidget&&) = delete;

        void toggleSelection();
        void toggleExplicitImplicit();
        bool isImplicitDrawn() const {return m_implicitDrawing;}


    protected:
        // -- from QWidget
        virtual void enterEvent(QEvent *event) override;
        virtual void leaveEvent(QEvent *event) override;
        virtual void mousePressEvent(QMouseEvent *event) override;
        // -- from DrumTabPartWidgetBase
        virtual void paintEvent(QPaintEvent* e) override;
        virtual QSize sizeHint() const override final;

    signals:
        void selected(DrumTabPartDisplayWidget* selectedWidget);
        void explicitImplicitToggled(DrumTabPartDisplayWidget* selectedWidget);
        void menuAddTabLinePressed(DrumTabPartDisplayWidget* sender,bool aboveBelow); //Above = true, Below=false
        void menuRemoveTabLinePressed(DrumTabPartDisplayWidget* sender);
        void menuAddTabPartPressed(DrumTabPartDisplayWidget* sender,bool leftRight); //Left = true, Right=false
        void menuRemoveTabPartPressed(DrumTabPartDisplayWidget* sender);

    public slots:

    private:
        // private drawing functions
        // -------------------------
        void drawBackground();

    private:
        // UI
        bool     m_selected{false};
        bool     m_implicitDrawing{false};
        double   m_zoomFactor{1.0};
        QAction *m_rightClickMenu_ExplicitImplicitToggle{nullptr};
        QAction *m_rightClickMenu_AddTabUnder{nullptr};
        QAction *m_rightClickMenu_AddTabOver{nullptr};
        QAction *m_rightClickMenu_RemoveTabLine{nullptr};
        QAction *m_rightClickMenu_AddTabPartLeft{nullptr};
        QAction *m_rightClickMenu_AddTabPartRight{nullptr};
        QAction *m_rightClickMenu_RemoveTabPart{nullptr};

    };

}

#endif // DRUMTABPARTWIDGET_H
