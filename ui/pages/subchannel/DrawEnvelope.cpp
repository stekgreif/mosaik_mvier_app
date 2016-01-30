#include "DrawEnvelope.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>




DrawEnvelope::DrawEnvelope(QSize size, QWidget *parent)
    : QWidget(parent)
{
    qDebug() <<Q_FUNC_INFO <<"Init.";

    m_widgetSize = new QSize;
    m_widgetSize->setHeight(size.height());
    m_widgetSize->setWidth(size.width());

    m_envelope = new envelope_t;
}



void DrawEnvelope::setEnvelope(envelope_t envelope)
{
    qDebug() <<Q_FUNC_INFO;
    m_envelope->start   = m_widgetSize->width() * envelope.start;
    m_envelope->fadeIn  = m_widgetSize->width() * envelope.fadeIn;
    m_envelope->fadeOut = m_widgetSize->width() * envelope.fadeOut;
    m_envelope->end     = m_widgetSize->width() * envelope.end;

    update();
}



void DrawEnvelope::refresh()
{
#if 0
    QPainter painter(this);
    QPen     line;
    QBrush   fill;
    QColor   fillColor;
    QColor   lineColor;

    line.setWidth(3);

    fillColor.setRgba(qRgba(20,20,20,20));
    lineColor.setRgb(200,200,200);

    line.setColor(lineColor);
    fill.setColor(fillColor);

    painter.setPen(line);
    painter.setBrush(fill);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPolygon poly(4);
    poly.setPoint(0, 0,0);
    poly.setPoint(1, (int)m_envelope->fadeIn,0);
    poly.setPoint(2, 0,m_widgetSize->height());
    poly.setPoint(3, (int)m_envelope->start,m_widgetSize->height());

    painter.drawPolygon(poly);

    //painter.drawLine(m_envelope->start, 0, m_envelope->start, m_widgetSize->height());

    update();
#endif
}




void DrawEnvelope::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption myOption;
    myOption.initFrom(this);
    QPainter myPainter(this);
    style()->drawPrimitive( QStyle::PE_Widget, &myOption, &myPainter, this);


    QPainter painter(this);
    QPen     line;
    QBrush   fill;
    QColor   fillColor;
    QColor   lineColor;

    line.setWidth(2);

    fillColor.setRgba(qRgba(0,0,0,140));
    lineColor.setRgb(0,0,0);

    line.setColor(lineColor);
    fill.setColor(fillColor);
    fill.setStyle(Qt::SolidPattern);

    //painter.setPen(line);
    painter.setBrush(fill);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPolygon polyStart(4);
    polyStart.setPoint(0, 0,0);
    polyStart.setPoint(1, (int)m_envelope->fadeIn,0);
    polyStart.setPoint(2, (int)m_envelope->start,m_widgetSize->height());
    polyStart.setPoint(3, 0,m_widgetSize->height());

    QPolygon polyEnd(4);
    polyEnd.setPoint(0, (int)m_envelope->fadeOut ,0);
    polyEnd.setPoint(1, m_widgetSize->width(), 0);
    polyEnd.setPoint(2, m_widgetSize->width(), m_widgetSize->height());
    polyEnd.setPoint(3, (int)m_envelope->end, m_widgetSize->height());


    painter.drawPolygon(polyStart);
    painter.drawPolygon(polyEnd);

    //painter.drawLine(m_envelope->start, 0, m_envelope->start, m_widgetSize->height());

}

