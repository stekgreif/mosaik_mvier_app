#include "PageTest.h"

#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QPainter>
#include <QStyleOption>


PageTest::PageTest(QWidget *parent) : QWidget(parent)
{
    qDebug() <<Q_FUNC_INFO <<"Init";
    this->setObjectName("pageTest");

    QScrollArea *m_scrollArea = new QScrollArea(this);
    m_scrollArea->setFixedSize(600,600);
    m_scrollArea->move(10,100);
    QLabel *m_toolbarDummy = new QLabel;
    QImage image("Overview.png");
    if(image.isNull())
        qDebug() <<Q_FUNC_INFO <<"image not found";
    m_toolbarDummy->setPixmap(QPixmap::fromImage(image));
    m_scrollArea->setWidget(m_toolbarDummy);


    QLabel *headline = new QLabel("Tests", this);
    headline->setObjectName("pageheadline");
    headline->move(40, 40);
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

