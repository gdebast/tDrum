#ifndef DRUMTABPARTWIDGETBASE_H
#define DRUMTABPARTWIDGETBASE_H

#include "DrumEnum/drum_drumkit.h"
#include "DrumUiTools/drumui_drumtabpartdrawerhelper.h"

#include <QWidget>
#include <QRect>
#include <QMenu>

namespace Drum {
    class DrumTabPart;
}

namespace DrumUI {

    /*
     * Widget displaying a DrumTabPart without any possibility of editing it.
     * It is used as bas class for DrumTabPartDisplayWidget and DrumTabPartCreatorWidget
     */

    class DrumTabPartWidgetBase : public QWidget
    {
        Q_OBJECT
    public:
        explicit DrumTabPartWidgetBase(Drum::DrumTabPart* drumTabPartModel,
                                       QWidget *parent = nullptr);

        Drum::DrumTabPart* getDrumTabPart() const {return m_drumTabPartModel;};
        void setDrumTabPart (const Drum::DrumTabPart& drumTabPart);

        static int getFixedHeight();
        static int getFixedWidth();

    protected:
        // -- from QWidget
        virtual void paintEvent(QPaintEvent* e) override;
        virtual QSize sizeHint() const override;

        // method to be called when the right mouse click is pressed
        virtual void rightMouseClickEvent(int eventX, int eventY) final;

    signals:
        void menuCopyPressed(const Drum::DrumTabPart& copiedDrumTabPart);
        void menuPastePressed(DrumTabPartWidgetBase* sender);

    private:

        // clear the model
        void emptyDrumTabPart (int newTime);

    protected:
        // model. It can be null because the widget can just display an empty part
        Drum::DrumTabPart *m_drumTabPartModel{nullptr};

        // helper object to know the position of a drumkit and its drawing rectangle
        DrumTabPartDrawerHelper m_drumTabPartDrawerHelper;

        // menu to copy-paste-clear the drum tab part
        QMenu   *m_rightClickMenu{nullptr};
        QAction *m_rightClickMenu_copy{nullptr};
        QAction *m_rightClickMenu_paste{nullptr};
        QAction *m_rightClickMenu_clear{nullptr};

    };

}

#endif // DRUMTABPARTWIDGETBASE_H
