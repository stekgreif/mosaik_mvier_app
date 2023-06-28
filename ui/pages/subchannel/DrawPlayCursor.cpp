#include "DrawPlayCursor.h"

#include <QPainter>
#include <QDebug>


DrawPlayCursor::DrawPlayCursor(QWidget *parent)
    : QWidget(parent)
    , m_background(new QWidget(this))
{
    qDebug() <<Q_FUNC_INFO <<"Init.";

    this->setFixedSize(620,320); // set widget size

    m_cursorPos = 100;
}


void DrawPlayCursor::setPlayCursorPosition(int pos)
{
    m_cursorPos = pos*6;
    this->update();
}


void DrawPlayCursor::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev);
    QPainter myPainter(this);
    QPen myPen;

    myPen.setWidth(2);

    myPainter.setPen(myPen);
    myPainter.drawLine(m_cursorPos, 0, m_cursorPos, 400);
}
