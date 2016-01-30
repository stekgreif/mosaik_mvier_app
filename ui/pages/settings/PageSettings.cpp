#include "PageSettings.h"

#include <QLabel>
#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QCheckBox>
#include <QStyleOption>



PageSettings::PageSettings(QWidget *parent) : QWidget(parent)
{
    qDebug() <<Q_FUNC_INFO <<"Init.";

    this->setObjectName("pageSettings");

    QLabel *headline = new QLabel("Settings", this);
    headline->setObjectName("pageheadline");
    headline->move(40, 40);

    //tooltip
    QCheckBox *checkBox;
    checkBox = new QCheckBox("Fast Selection", this);
    checkBox->setObjectName("settings_checkbox");
    checkBox->move(200,200);

}



PageSettings::~PageSettings()
{

}



void PageSettings::refresh()
{

}



void PageSettings::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption myOption;
    myOption.initFrom(this);
    QPainter myPainter(this);
    style()->drawPrimitive( QStyle::PE_Widget, &myOption, &myPainter,this);
}

