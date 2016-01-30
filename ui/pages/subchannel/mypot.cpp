#include "mypot.h"

#include <QDebug>
#include <QPainter>



MyPot::MyPot(QWidget *parent)
    : QDial(parent)
{
    //qDebug() <<Q_FUNC_INFO <<"Init.";
}


void MyPot::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QPen     myPen;
    QColor   myColor;

    int val = QDial::value();

    //myColor.setRgb(255,131,0);
    myColor.setRgb(0,0,0);
    myPen.setWidth(4);
    myPen.setColor(myColor);
    myPen.setCapStyle(Qt::RoundCap);


    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(myPen);

    //painter.drawEllipse(5,5,60,60);
    painter.drawArc(5,5,60,60, 300.0*16, 300.0*16);

    painter.translate( width()/2, height()/2 );
    painter.rotate(30);
    painter.rotate(val * 3.0);
    painter.drawLine(0,0,0,30);
}


