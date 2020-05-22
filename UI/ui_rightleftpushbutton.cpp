#include "UI/ui_rightleftpushbutton.h"

using namespace UI;

RightLeftPushButton::RightLeftPushButton(QWidget *parent) : QPushButton(parent)
{

}

void RightLeftPushButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        emit rightClicked();
    }
    else if (event->button() == Qt::LeftButton)
    {
        emit leftClick();
    }
}
