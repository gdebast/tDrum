#pragma once

#include <QPushButton>
#include <QMouseEvent>

namespace UI
{
    class RightLeftPushButton : public QPushButton
    {
        Q_OBJECT
        public:
            explicit RightLeftPushButton(QWidget *parent = nullptr);

        protected:
            virtual void mousePressEvent(QMouseEvent *event) override;

        signals:
            void rightClicked();
            void leftClick();

    };
}



