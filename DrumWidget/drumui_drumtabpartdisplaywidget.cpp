#include "DrumWidget/drumui_drumtabpartdisplaywidget.h"
#include "DrumAPI/drum_drumtabpart.h"

#include <QMouseEvent>
#include <QPainter>

using namespace DrumUI;


// =================
// == Constructor ==
// =================

DrumTabPartDisplayWidget::DrumTabPartDisplayWidget(Drum::DrumTabPart* drumTabPartModel,
                                                   bool implicitDrawing,
                                                   QWidget *parent)
    : DrumTabPartWidgetBase(drumTabPartModel,parent),
      m_selected(false),
      m_implicitDrawing(implicitDrawing)
{

    // add some menu item to the base class menu
    m_rightClickMenu_ExplicitImplicitToggle = m_rightClickMenu->addAction(m_implicitDrawing? "set explicit" : "set implicit");
    auto *addTabLineSection = m_rightClickMenu->addMenu("tab line");
    m_rightClickMenu_AddTabOver = addTabLineSection->addAction("add above");
    m_rightClickMenu_AddTabUnder = addTabLineSection->addAction("add below");
    m_rightClickMenu_RemoveTabLine = addTabLineSection->addAction("remove");
    auto *addTabPartSection = m_rightClickMenu->addMenu("tab part");
    m_rightClickMenu_AddTabPartLeft = addTabPartSection->addAction("add left");
    m_rightClickMenu_AddTabPartRight = addTabPartSection->addAction("add right");
    m_rightClickMenu_RemoveTabPart = addTabPartSection->addAction("remove");

    // connection
    QObject::connect(m_rightClickMenu_AddTabOver,&QAction::triggered,this,[this](){emit menuAddTabLinePressed(this,true);});
    QObject::connect(m_rightClickMenu_AddTabUnder,&QAction::triggered,this,[this](){emit menuAddTabLinePressed(this,false);});
    QObject::connect(m_rightClickMenu_RemoveTabLine,&QAction::triggered,this,[this](){emit menuRemoveTabLinePressed(this);});
    QObject::connect(m_rightClickMenu_ExplicitImplicitToggle,&QAction::triggered,this,[this](){emit explicitImplicitToggled(this);});
    QObject::connect(m_rightClickMenu_AddTabPartLeft,&QAction::triggered,this,[this](){emit menuAddTabPartPressed(this,true);});
    QObject::connect(m_rightClickMenu_AddTabPartRight,&QAction::triggered,this,[this](){emit menuAddTabPartPressed(this,false);});
    QObject::connect(m_rightClickMenu_RemoveTabPart,&QAction::triggered,this,[this](){emit menuRemoveTabPartPressed(this);});

    QSizePolicy policy(QSizePolicy::Policy::Fixed,QSizePolicy::Policy::Fixed);
    setSizePolicy(policy);

    // icon
    m_rightClickMenu_ExplicitImplicitToggle->setIcon(QIcon(":/tDrum_explicitImplicit_drumtabpart.svg"));


}

// ======================
// == public functions ==
// ======================

void DrumTabPartDisplayWidget::toggleSelection()
{
    m_selected = !m_selected;
    repaint();
}

void DrumTabPartDisplayWidget::toggleExplicitImplicit()
{
    m_implicitDrawing = !m_implicitDrawing;
    m_rightClickMenu_ExplicitImplicitToggle->setText(m_implicitDrawing? "set explicit" : "set implicit");
    repaint();
}

// =========================
// == protected functions ==
// =========================


void DrumTabPartDisplayWidget::enterEvent(QEvent*)
{
    repaint();
}

void DrumTabPartDisplayWidget::leaveEvent(QEvent*)
{
    repaint();
}

void DrumTabPartDisplayWidget::mousePressEvent(QMouseEvent* event)
{
    emit selected(this);

    if(event->button() == Qt::RightButton)
    {
        DrumTabPartWidgetBase::rightMouseClickEvent(event->x(), event->y());
    }

}

void DrumTabPartDisplayWidget::paintEvent(QPaintEvent* e)
{

    // draw the background base
    drawBackground();


    // draw from DrumTabPartWidgetBase
    DrumTabPartWidgetBase::paintEvent(e);

}

// ===============================
// == private drawing functions ==
// ===============================


void DrumTabPartDisplayWidget::drawBackground()
{
    QPainter p(this);

    bool isUnderMouse(underMouse());
    DrumTabPartGeometry geo(width(),height());

    // colors
    QColor selectedBlue = QColor(180,200,255);
    QColor hoveredBlue = QColor(220,220,220);
    QColor implicitGreen = QColor(216,255,220);
    QColor implictANDselected = QColor(42,148,81);
    QColor implictANDhovered = QColor(120,181,126);

    // draw the general back ground
    if(m_selected)
        p.fillRect(QRect(0,0,width(),height()),selectedBlue);
    else if(isUnderMouse)
        p.fillRect(QRect(0,0,width(),height()),hoveredBlue);

    if (m_drumTabPartModel->isEmpty())
        return;

    // draw the rectangle in the grid based on the implicit/explicit status
    QRect gridRect(0,geo.getHiHatLine(),width(),
                   geo.getBassDrumLine()-geo.getHiHatLine());
    if(m_selected && m_implicitDrawing == false)
        p.fillRect(gridRect,implictANDselected);
    else if (isUnderMouse && m_implicitDrawing == false)
        p.fillRect(gridRect,implictANDhovered);
    else if (m_implicitDrawing == false)
        p.fillRect(gridRect,implicitGreen);


}


