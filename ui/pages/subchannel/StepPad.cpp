#include "StepPad.h"
#include <QDebug>
#include <QWidget>
#include <core/subchannel/SubchannelManager.h>


StepPad::StepPad(int id, QWidget *parent)
    : QWidget(parent)
{
    //qDebug() <<Q_FUNC_INFO <<"Init";

    m_id = id;
    m_colorPad  = new QWidget(this);
    m_colorPad->setObjectName("colorPad");

    m_buttonPad = new QPushButton(this);
    m_buttonPad->setObjectName("buttonPad");
    m_buttonPad->setStyleSheet("QPushButton#buttonPad {background-color: rgba(20,20,25,0);}");

    connect(m_buttonPad, SIGNAL(pressed()), this, SLOT(slot_btnPressed()) );

    clrStep();
}


StepPad::~StepPad()
{

}


void StepPad::setStep()
{
    //m_colorPad->setStyleSheet("QWidget#colorPad {background-color: rgb(120,20,25);}");
    switch (subchannelManager().getCurrentSubchannelSelection()%4)
    {
        case 0:  //red 255,0,0
            m_colorPad->setStyleSheet("QWidget#colorPad {background-color: rgba(255,0,0,50%);}");
            break;
        case 1: //blue 0,0,255
            m_colorPad->setStyleSheet("QWidget#colorPad {background-color: rgba(0,0,255,50%);}");
            break;
        case 2: //yellow 255,255,0
            m_colorPad->setStyleSheet("QWidget#colorPad {background-color: rgba(255,255,0,50%);}");
            break;
        case 3: //magenta 255,0,255
            m_colorPad->setStyleSheet("QWidget#colorPad {background-color: rgba(255,0,255,50%);}");
            break;
        default:
            break;
    }
}


void StepPad::clrStep()
{
    m_colorPad->setStyleSheet("QWidget#colorPad {background-color: rgb(40,40,45);}");
}


void StepPad::setSize(int x, int y)
{
    m_colorPad->setFixedSize(x,y);
    m_buttonPad->setFixedSize(x,y);
}


void StepPad::slot_btnPressed()
{
    qDebug() <<Q_FUNC_INFO <<"pad pressed";
    emit signal_padPressed(m_id);
}


void StepPad::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption myOption;
    myOption.initFrom(this);
    QPainter myPainter(this);
    style()->drawPrimitive( QStyle::PE_Widget, &myOption, &myPainter,this);
}


