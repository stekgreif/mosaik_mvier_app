#include "rotary.h"
#include <QPainter>
#include <QStyleOption>
#include <QDebug>


///@todo 25 - simplify: combine rotary and mypot


Rotary::Rotary(int id, QWidget *parent)
    : QWidget(parent)
    , m_labelName(new QLabel(this))
    , m_labelValue(new QLabel(this))
    , m_poti(new MyPot(this))
{
    //qDebug() <<Q_FUNC_INFO <<"Init.";

    m_id = id;

    this->setObjectName("rotary");
    this->setStyleSheet("QWidget#rotary {background-color: rgb(150,150,160)}");
    this->setFixedSize(80,130);

    m_isSelected = false;

    m_poti->setFixedSize(70,70);
    m_poti->move(5,5);
    m_poti->setRange(0,100);

    m_labelValue->setFixedSize(70,20);
    m_labelValue->move(5,80);
    m_labelValue->setAlignment(Qt::AlignCenter);
    m_labelValue->setText("0.0");
    m_labelValue->setStyleSheet("QLabel{"
                                "background-color: black;"
                                "font-family: Droid Sans Mono;"
                                "color: white;"
                                "font-size: 16px }" );

    m_labelName->setFixedSize(70,20);
    m_labelName->move(5,100);
    m_labelName->setText("Name");
    m_labelName->setAlignment(Qt::AlignCenter);
    m_labelName->setStyleSheet("QLabel{ "
                               "background-color: white;"
                               "font-family: Droid Sans;"
                               "font-size: 16px }" );

    slot_setValue(0.0);

    connect( m_poti, SIGNAL( valueChanged(int) ), this,  SLOT( slot_potValChanged(int) ) );
}



void Rotary::slot_setSelection(bool state)
{
    m_isSelected = state;
}

void Rotary::slot_setName(QString name)
{
    m_labelName->setText(name);
}

void Rotary::slot_setValue(double value)
{
    m_labelValue->setText(QString::number( value, 'f', 2 ) );
    m_poti->update();
}



/// @todo 24 - could this be in a higher resolution?
void Rotary::slot_potValChanged(int val)
{
    qDebug() <<Q_FUNC_INFO <<"rotary" <<m_id <<"changed to:" <<val;
    emit signal_valueChanged(m_id, ((float)val)/100 );
}





void Rotary::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption myOption;
    myOption.initFrom(this);
    QPainter myPainter(this);
    style()->drawPrimitive( QStyle::PE_Widget, &myOption, &myPainter, this);

    QPainter painter(this);
    QPen     selectionPen;
    QColor   myColor;

    if( m_isSelected == true )
    {
        myColor.setRgb(255,131,0);
    }
    else
    {
        myColor.setRgb(150,150,160);
    }

    selectionPen.setWidth(4);
    selectionPen.setColor(myColor);
    painter.setPen(selectionPen);
    painter.drawRect(2,2, 76, 126);

}
