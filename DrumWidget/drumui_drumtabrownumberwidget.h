#pragma once

#include <QWidget>


namespace DrumUI
{
    class DrumTabRowNumberWidget : public QWidget
    {
            Q_OBJECT
        public:
            explicit DrumTabRowNumberWidget(unsigned displayedRowNumber, double zoomFactor, QWidget *parent = nullptr);

        private:
            // -- from QWidget
            virtual void paintEvent(QPaintEvent* e) override final;
            virtual QSize sizeHint() const override final;

            unsigned m_displayedRowNumber;
            double   m_zoomFactor;


    };


}
