#pragma once

#include "DrumWidget/drumui_drumtabpartwidgetbase.h"

#include "QRect"

namespace Drum {
    class DrumTabPart;
}

namespace DrumUI {

    /*
     * Widget responsible for creating a DrumTabPart
     * the displaying responsability is handed to DrumTabPartWidgetBase
     */

    class DrumTabPartCreatorWidget final : public DrumTabPartWidgetBase
    {
    Q_OBJECT
    public:
        DrumTabPartCreatorWidget(Drum::DrumTabPart& drumTabPartModel,
                                 QWidget *parent = nullptr);

        DrumTabPartCreatorWidget(const DrumTabPartCreatorWidget&) = delete;
        DrumTabPartCreatorWidget(DrumTabPartCreatorWidget&&) = delete;

    protected:
        virtual void paintEvent(QPaintEvent *e) override;
        virtual QSize sizeHint() const override;
        virtual void mouseMoveEvent(QMouseEvent *event) override;
        virtual void mousePressEvent(QMouseEvent *event) override;

    private:

        // add a note at the drum tab part
        void leftMouseClickEvent(int eventX, int eventY);

        // method to paint the hoovered/selected rectangle
        void paintHooveredSelectedQRect();

        // rectangle in the Widget which is currently hoovered/selected
        QRect *m_hooveredSelectedQRect{nullptr};

    };
}

