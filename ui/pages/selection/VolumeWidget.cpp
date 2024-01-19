#include "VolumeWidget.h"

#include <QPainter>
#include <QStyleOption>
#include <QDebug>

#include "core/subchannel/SubchannelManager.h"


VolumeWidget::VolumeWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setObjectName("VolumeWidget");
    this->setFixedSize(5, 110);

    m_value = 0.8;
}


VolumeWidget::~VolumeWidget()
{

}


void VolumeWidget::setVolume(float vol)
{
    m_value = vol;
    this->update();
}


void VolumeWidget::paintEvent(QPaintEvent *event)
{
    QPainter backline(this);
    backline.setPen(QPen( QColor( 109,109,109), 1) );
    backline.drawLine(3, 0, 3, 110);

    QPainter volLine(this);
    volLine.setPen(QPen( QColor( 158,158,158), 3) );
    volLine.drawLine(3, 110*(1-m_value) , 3,110);

    Q_UNUSED(event);
    QStyleOption myOption;
    myOption.initFrom(this);
    QPainter myPainter(this);
    style()->drawPrimitive( QStyle::PE_Widget, &myOption, &myPainter, this);
}
