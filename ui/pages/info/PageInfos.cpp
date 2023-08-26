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

    QLabel *headline = new QLabel("Keys", this);
    headline->setObjectName("pageheadline");
    headline->move(40, 40);

    QLabel *textbody = new QLabel("ESC: Exit\n"
                                  "RETURN: load Sample\n"
                                  "F1: Screen size normal\n"
                                  "F2: Full Screen\n"
                                  "F5: Sample and Browser View\n"
                                  "F6: Info page (this)\n"
                                  "F7: Toolbox Demo View\n"
                                  "F12: Set Step LED 11\n"
                                  "1: Step Button 1\n"
                                  "2: Step Button 17\n"
                                  "3: Step Button 33\n"
                                  "4: Step Button 48\n"
                                  "6: Print Pattern to console\n"
                                  "7: Connect to MIDI Device\n"
                                  "8: Send out Pattern via MIDI\n"
                                  "9: Toggle Step 3\n"
                                  "0: unload sample\n", this);
    textbody->setObjectName("textbody");
    textbody->move(40,80);
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


