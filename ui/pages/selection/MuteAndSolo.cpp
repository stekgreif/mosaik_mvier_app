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


    /** sizes **/
    m_pageWidth = settings().getScreenSize().height();
    m_gridWidth = m_pageWidth / settings().getNumberOfSubchannelsPerRow();
    m_padWidth  = (m_gridWidth / 100) * 97;
    m_padGap    = (m_gridWidth / 100) * 3;

    setFixedSize( m_pageWidth, m_pageWidth );

    QSize *padSize;
    padSize = new QSize;
    padSize->setWidth(m_padWidth);
    padSize->setHeight(m_padWidth);


    /** connect pads **/
    int abs = 0;
    for(int i = 0; i < SETTINGS_SUBS_PER_COL; i++)
    {
        for(int j = 0; j < SETTINGS_SUBS_PER_ROW; j++)
        {
            abs = j+(i*SETTINGS_SUBS_PER_COL);
            m_muteAndSoloPad[abs] = new MuteAndSoloPad(abs, padSize, this);
            m_muteAndSoloPad[abs]->move(j * (m_padWidth + m_padGap), i * (m_padWidth + m_padGap));
            connect(m_muteAndSoloPad[abs], SIGNAL(signal_mutePadPressed(int)),
                    this,                  SLOT(slot_stateChanged(int)) );
        }
    }
}



MuteAndSolo::~MuteAndSolo()
{

}



void MuteAndSolo::display(bool state)
{
    Q_UNUSED(state);
    qDebug() <<Q_FUNC_INFO <<"Not implemented.";
}



void MuteAndSolo::refresh()
{
    QBitArray isMute(SETTINGS_NUM_OF_SUBS);
    isMute = subchannelManager().getMuteStates();

    for( int id = 0; id < SETTINGS_NUM_OF_SUBS; id++ )
    {
        if( isMute.at(id) )
            m_muteAndSoloPad[id]->slot_setPadToMuteColor();
        else
            m_muteAndSoloPad[id]->slot_setPadToUnmuteColor();
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
