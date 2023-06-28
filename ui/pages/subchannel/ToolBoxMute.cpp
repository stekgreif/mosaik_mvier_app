#include "ToolBoxMute.h"
#include <QPainter>
#include <QStyleOption>



ToolBoxMute::ToolBoxMute(QWidget *uiParent)	: QWidget(uiParent)
{
	m_testLabel = new QLabel(this);
	m_testLabel->setText("daniel");

	m_buttonMute = new QPushButton(this);
	m_buttonMute->setFixedSize(70,70);
	m_buttonMute->setText("MUTE");
	m_buttonMute->setCheckable(true);
}


ToolBoxMute::~ToolBoxMute()
{

}


void ToolBoxMute::paintEvent( QPaintEvent *event )
{
    Q_UNUSED( event );
	QStyleOption myOption;
    myOption.initFrom( this );
    QPainter myPainter( this );
    style()->drawPrimitive( QStyle::PE_Widget, &myOption, &myPainter, this );
}

