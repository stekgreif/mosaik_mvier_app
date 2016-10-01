#include "PageTest.h"

#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QPainter>
#include <QStyleOption>

#include "Settings.h"


PageTest::PageTest(QWidget *parent) : QWidget(parent)
{
    qDebug() <<Q_FUNC_INFO <<"Init";
    this->setObjectName("pageTest");

    QSize size;
    size.setWidth(settings().getScreenSize().height() /  1.3);
    size.setHeight(settings().getScreenSize().height() - 50);

    //QScrollArea *m_scrollArea = new QScrollArea(this);
    QGraphicsScene *m_scene = new QGraphicsScene(this);
    QGraphicsView *m_view = new QGraphicsView(this);

    QImage image(":/Overview.png");
    if(image.isNull())
        qDebug() <<Q_FUNC_INFO <<"image not found";
    QGraphicsPixmapItem *m_pixmap = new QGraphicsPixmapItem(QPixmap::fromImage(image.scaled(size.width()-20, 2600)));

    m_scene->addItem(m_pixmap);
    m_view->setFixedSize(size);
    m_view->setScene(m_scene);
    m_view->setDragMode(QGraphicsView::ScrollHandDrag);
    m_view->show();




    //m_toolbarDummy->setPixmap(QPixmap::fromImage(image.scaledToWidth(settings().getScreenSize().height() /  1.3)));
    //m_toolbarDummy->setScaledContents(true);
    //m_scrollArea->setWidget(m_toolbarDummy);


#if 0
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

