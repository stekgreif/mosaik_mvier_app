#include "VolumeWidget.h"

#include <QPainter>
#include <QStyleOption>
#include <QDebug>

#include "core/subchannel/SubchannelManager.h"


VolumeWidget::VolumeWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setObjectName("VolumeWidget");
    this->setFixedSize(5, 80);

    m_value = 0.0;
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
    backline.drawLine(3, 0, 3, 80);

    QPainter volLine(this);
    volLine.setPen(QPen( QColor( 158,158,158), 3) );
    volLine.drawLine(3, 80*(1-m_value) , 3, 80);

    Q_UNUSED(event);
    QStyleOption myOption;
    myOption.initFrom(this);
    QPainter myPainter(this);
    style()->drawPrimitive( QStyle::PE_Widget, &myOption, &myPainter, this);
}
