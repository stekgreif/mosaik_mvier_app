#include "MuteAndSoloPad.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>



MuteAndSoloPad::MuteAndSoloPad(int id, QSize *widgetSize, QWidget *parent)
    : QWidget(parent)
{
    m_id = id;

    m_widgetSize = new QSize;
    m_widgetSize->setHeight(widgetSize->height());
    m_widgetSize->setWidth(widgetSize->width());

    this->setObjectName("padWidget");
    this->setFixedSize(*m_widgetSize);

    m_buttonPadColor = new QLabel(this);
    m_buttonPadColor->setObjectName("buttonPadColor");
    m_buttonPadColor->setFixedSize(*m_widgetSize);
    slot_setPadToUnmuteColor();

    m_buttonPad = new QPushButton(this);
    m_buttonPad->setObjectName("buttonPad");
    m_buttonPad->setStyleSheet("QPushButton#buttonPad {background-color: rgba(0,0,0, 40%);}");
    m_buttonPad->setFixedSize(*m_widgetSize);
    m_buttonPad->setText(QString::number(m_id));

    /* internal connections */
    connect(m_buttonPad, SIGNAL(pressed()),  this, SLOT(slot_padPressed()) );
}

MuteAndSoloPad::~MuteAndSoloPad()
{
    delete m_buttonPad;
    delete m_buttonPadColor;
}



void MuteAndSoloPad::slot_setPadToMuteColor()
{
    m_buttonPadColor->setStyleSheet("QLabel#buttonPadColor {background-color: rgba(255,0,0, 100);}");
}

void MuteAndSoloPad::slot_setPadToUnmuteColor()
{
    m_buttonPadColor->setStyleSheet("QLabel#buttonPadColor {background-color: rgba(0,0,0,0);}");
}

void MuteAndSoloPad::slot_padPressed()
{
    emit signal_mutePadPressed(m_id);
}



void MuteAndSoloPad::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption myOption;
    myOption.initFrom(this);
    QPainter myPainter(this);
    style()->drawPrimitive( QStyle::PE_Widget, &myOption, &myPainter, this);
}
