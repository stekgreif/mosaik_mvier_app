#include "StepPad.h"

#include <QDebug>
#include <QWidget>



StepPad::StepPad(int id, QWidget *parent)
    : QWidget(parent)
{
    //qDebug() <<Q_FUNC_INFO <<"Init";

#if 0 // not working
    this->setObjectName("stepPad");
    this->setStyleSheet("StepPad#stepPad {background-color: rgb(130,30,30);}");
    this->setStyleSheet("QWidget#stepPad {background-color: rgb(130,30,30);}");
#endif

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
    m_colorPad->setStyleSheet("QWidget#colorPad {background-color: rgb(120,20,25);}");
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


