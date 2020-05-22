#include "DrumWidget/drumui_drumtabpartwidgetbase.h"

#include "DrumAPI/drum_drumtabpart.h"
#include "DrumAPI/drum_drumline.h"
#include "DrumUiTools/drumui_drumtabpartgeometry.h"
#include "DrumUiTools/drumui_drumtabpartdrawer.h"

#include <QPainter>

using namespace DrumUI;

// =================
// == Constructor ==
// =================

DrumTabPartWidgetBase::DrumTabPartWidgetBase(Drum::DrumTabPart* drumTabPartModel,
                                             QWidget *parent) :
    QWidget(parent),
    m_drumTabPartModel(drumTabPartModel),
    m_drumTabPartDrawerHelper(QRect(0,0,getFixedWidth(),getFixedHeight()),
                                     drumTabPartModel ? drumTabPartModel->getDrumTime() : 0)
{
    // menu
    m_rightClickMenu = new QMenu(this);
    m_rightClickMenu_copy  = m_rightClickMenu->addAction("copy");
    m_rightClickMenu_copy->setIcon(QIcon(":/tDrum_copy_drumtabpart_doubleleaf.svg"));
    m_rightClickMenu_paste = m_rightClickMenu->addAction("paste");
    m_rightClickMenu_paste->setIcon(QIcon(":/tDrum_paste_drumtabpart_bean.svg"));
    m_rightClickMenu_clear = m_rightClickMenu->addAction("clear");
    m_rightClickMenu_clear->setIcon(QIcon(":/tDrum_clear_drumtabpart_fire.svg"));

    // connection
    QObject::connect(m_rightClickMenu_copy,&QAction::triggered,this,[this](){emit this->menuCopyPressed(*m_drumTabPartModel);});
    QObject::connect(m_rightClickMenu_paste,&QAction::triggered,this,[this](){emit this->menuPastePressed(this);});
    QObject::connect(m_rightClickMenu_clear,
                     &QAction::triggered,
                     this,
                     [this]()
                        {
                        int time = m_drumTabPartModel->getDrumTime();
                        emptyDrumTabPart(time);
                        });



}

void DrumTabPartWidgetBase::setDrumTabPart(const Drum::DrumTabPart &drumTabPart)
{
    m_drumTabPartModel->operator=(drumTabPart);
    repaint();
}

void DrumTabPartWidgetBase::emptyDrumTabPart(int newTime)
{
    m_drumTabPartModel->clear();
    m_drumTabPartModel->setDrumTime(newTime);
    repaint();
}

// ======================
// == public functions ==
// ======================

int DrumTabPartWidgetBase::getFixedHeight()
{
    return DrumTabPartGeometry::getBaseHeight();
}

int DrumTabPartWidgetBase::getFixedWidth()
{
    return DrumTabPartGeometry::getBaseWidght();
}


// =========================
// == protected functions ==
// =========================

QSize DrumTabPartWidgetBase::sizeHint() const
{
    return QSize(getFixedWidth(), getFixedHeight());
}

void DrumTabPartWidgetBase::rightMouseClickEvent(int eventX, int eventY)
{
    m_rightClickMenu->exec(mapToGlobal(QPoint(eventX,eventY)));
}



void DrumTabPartWidgetBase::paintEvent(QPaintEvent* e)
{
    QWidget::paintEvent(e);

    // recreate it because the model might have changed
    m_drumTabPartDrawerHelper.recompute(QRect(0,0,width(),height()),
                                        m_drumTabPartModel ? m_drumTabPartModel->getDrumTime() : 0);


    DrumTabPartDrawer drawer(*this, *m_drumTabPartModel);
    drawer.DrawDrumTabPart(m_drumTabPartDrawerHelper);

}
