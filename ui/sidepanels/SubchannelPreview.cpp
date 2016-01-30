#include "SubchannelPreview.h"

#include <QDebug>

#include "Settings.h"
#include "core/subchannel/SubchannelManager.h"



SubchannelPreview::SubchannelPreview(QWidget *parent)
    : QWidget(parent)
{
    qDebug() <<Q_FUNC_INFO <<"Init.";

    this->setFixedSize(215,215);
    this->setObjectName("subchannelPreview");
    //this->setStyleSheet("SubchannelPreview#subchannelPreview {background-color: rgb(60,60,60)}");

    int pos = 0;
    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            pos = x + (y * 8);
            m_boxes[pos] = new QLabel(QString::number(settings().getSubchannelId(pos)), this);
            m_boxes[pos]->setObjectName("subchannelPreviewBox");
            m_boxes[pos]->setFixedSize(20,20);
            m_boxes[pos]->setStyleSheet("QLabel#subchannelPreviewBox {background-color: rgb(160,160,160)}");
            m_boxes[pos]->move(x*25+10, y*25+10);
        }
    }
}



SubchannelPreview::~SubchannelPreview()
{

}

void SubchannelPreview::refresh()
{
    clearAll();
    setSubchannelSelection(subchannelManager().getCurrentSubchannelSelection());
}

void SubchannelPreview::clearAll()
{
    for(int i = 0; i < 64; i++)
        m_boxes[i]->setStyleSheet("QLabel#subchannelPreviewBox {background-color: rgb(160,160,160)}");
}

void SubchannelPreview::setSubchannelSelection(int id)
{
    int pos = settings().getSubchannelPos(id);
    m_boxes[pos]->setStyleSheet("QLabel#subchannelPreviewBox {background-color: rgb(255,140,0)}");
}




void SubchannelPreview::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption myOption;
    myOption.initFrom(this);
    QPainter myPainter(this);
    style()->drawPrimitive( QStyle::PE_Widget, &myOption, &myPainter,this);
}



void SubchannelPreview::slot_changeSubchannel(int id)
{
    qDebug() <<Q_FUNC_INFO <<"Subchannel changed" <<id;
    if((id >= 0) && (id <64))
    {
        clearAll();
    }
    setSubchannelSelection(id);
}

