#include "PageDebug.h"
#include <QDebug>



PageDebug::PageDebug(QWidget *parent) : QWidget(parent)
{
    qDebug() <<Q_FUNC_INFO <<"Init.";
}



PageDebug::~PageDebug()
{
    qDebug() <<Q_FUNC_INFO <<"Deinit.";
}



void PageDebug::refresh()
{

}

