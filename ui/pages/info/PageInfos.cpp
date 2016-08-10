#include "PageInfos.h"

#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QPainter>
#include <QStyleOption>



PageInfos::PageInfos(QWidget *parent) : QWidget(parent)
{
    qDebug() <<Q_FUNC_INFO <<"Init";

    this->setObjectName("pageInfos");

    QLabel *headline = new QLabel("Infos", this);
    headline->setObjectName("pageheadline");
    headline->move(40, 40);
}



PageInfos::~PageInfos()
{

}



void PageInfos::refresh()
{

}



void PageInfos::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption myOption;
    myOption.initFrom(this);
    QPainter myPainter(this);
    style()->drawPrimitive( QStyle::PE_Widget, &myOption, &myPainter,this);
}


