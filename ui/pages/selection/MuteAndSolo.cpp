#include "MuteAndSolo.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include "Settings.h"
#include "core/subchannel/SubchannelManager.h"

#define BTN_SIZE  131
#define BTN_GAP     4
#define OFFSET      2
#define BTN_GRID  (BTN_SIZE+BTN_GAP)
#define BTNS  8



MuteAndSolo::MuteAndSolo(QWidget *parent)
    : QWidget(parent)
{
    qDebug() <<Q_FUNC_INFO <<"Created.";
    setObjectName("muteAndSolo");
    setStyleSheet("QWidget#muteAndSolo {background-color: rgba(100,100,100,50%);}");
    setFixedSize(settings().getScreenSize().height(), settings().getScreenSize().height());

    QSize *padSize;
    padSize = new QSize;
    padSize->setWidth(BTN_SIZE);
    padSize->setHeight(BTN_SIZE);
#if 1
    int abs = 0;
    for(int i = 0; i < BTNS; i++)
    {
        for(int j = 0; j < BTNS; j++)
        {
            abs = j+(i*8);
            m_muteAndSoloPad[abs] = new MuteAndSoloPad(settings().getSubchannelId(abs), padSize, this);
            m_muteAndSoloPad[abs]->move(j * BTN_GRID + OFFSET, i * BTN_GRID + OFFSET);
            connect(m_muteAndSoloPad[abs], SIGNAL(signal_mutePadPressed(int)),
                    this,                  SLOT(slot_stateChanged(int)) );
        }
    }
#endif

}

MuteAndSolo::~MuteAndSolo()
{

}



void MuteAndSolo::display(bool state)
{
    Q_UNUSED(state);
#if 0
    if( state )
        this->setFixedSize(settings().getScreenSize().height(), settings().getScreenSize().height());
    else
        this->setFixedSize(1,1);

    update();
#endif
}



void MuteAndSolo::refresh()
{
    QBitArray isMute(64);
    isMute = subchannelManager().getMuteStates();

    for( int id = 0; id < 64; id++ )
    {
        if( isMute.at(id) )
            m_muteAndSoloPad[settings().getSubchannelPos(id)]->slot_setPadToMuteColor();
        else
            m_muteAndSoloPad[settings().getSubchannelPos(id)]->slot_setPadToUnmuteColor();
    }
}



void MuteAndSolo::slot_stateChanged(int id)
{
    qDebug() <<Q_FUNC_INFO <<"id" <<id;
    //emit signal_mutePadPressed(id);
    subchannelManager().toggleMuteState(id);
    refresh();
}



void MuteAndSolo::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption myOption;
    myOption.initFrom(this);
    QPainter myPainter(this);
    style()->drawPrimitive( QStyle::PE_Widget, &myOption, &myPainter, this);
}
