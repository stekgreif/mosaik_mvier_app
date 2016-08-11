#include "PageTest.h"

#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QPainter>
#include <QStyleOption>


PageTest::PageTest(QWidget *parent) : QWidget(parent)
{
#if 0
    qDebug() <<Q_FUNC_INFO <<"Init";
    this->setObjectName("pageTest");

    QLabel *headline = new QLabel("Tests", this);
    headline->setObjectName("pageheadline");
    headline->move(40, 40);
#endif
}


PageTest::~PageTest()
{

}

void PageTest::refresh()
{

}


void PageTest::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption myOption;
    myOption.initFrom(this);
    QPainter myPainter(this);
    style()->drawPrimitive( QStyle::PE_Widget, &myOption, &myPainter,this);
}

