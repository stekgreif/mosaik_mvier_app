#include "Settings.h"

#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QStyleOption>
#include <QBitArray>


#include <ui/UiManager.h>
#include "ui/pages/selection/PageSelection.h"
#include <core/subchannel/SubchannelManager.h>



#define BTN_SIZE  131
#define BTN_GAP     4
#define OFFSET      2
#define BTN_GRID  (BTN_SIZE+BTN_GAP)
#define BTNS  8

#define TIMER 1


PageSelection::PageSelection(UiManager *parent)
    : QWidget(parent)
    , m_muteAndSoloVisible(false)
{
    qDebug() <<Q_FUNC_INFO <<"Init.";
    m_lastSelectedSubchannel = 0;

    m_channelSelectionFrame = new QLabel(this);
    m_channelSelectionFrame->setFixedSize(BTN_GRID*2, BTN_GRID*2);
    m_channelSelectionFrame->setObjectName("channelSelectionFrame");

    QSize *padSize;
    padSize = new QSize;
    padSize->setWidth(BTN_SIZE);
    padSize->setHeight(BTN_SIZE);

    int abs = 0;
    for(int i = 0; i < BTNS; i++)
    {
        for(int j = 0; j < BTNS; j++)
        {
            abs = j+(i*8);
            m_pad[abs] = new SelectionPad(settings().getSubchannelId(abs), padSize, this);
            m_pad[abs]->move(j * BTN_GRID + OFFSET, i * BTN_GRID + OFFSET);
            connect(m_pad[abs], SIGNAL(signal_subchPadPressed(int)), this, SLOT(m_slot_selectionChanged(int)) );
        }
    }
    m_muteAndSolo = new MuteAndSolo(this);
    m_muteAndSolo->setFixedSize(1,1);
    this->update();
    //m_muteAndSolo->display(false);
    qDebug() <<Q_FUNC_INFO <<BTNS*BTNS <<"pads created.";


    m_timer = new QTimer;
    connect( m_timer, SIGNAL(timeout()), this,  SLOT(slot_regularTimer()) );
    m_timer->start(50);


    refresh();
}



PageSelection::~PageSelection()
{
    int abs = 0;
    for(int i = 0; i < BTNS; i++)
    {
        for(int j = 0; j < BTNS; j++)
        {
            abs = j+(i*8);
            delete m_pad[abs];
        }
    }

    delete m_channelSelectionFrame;
    qDebug() <<"~" <<Q_FUNC_INFO;
}



/** QTimer **/
void PageSelection::slot_regularTimer()
{
#if TIMER

    QBitArray hasSample = subchannelManager().hasSample();
    QBitArray hasSteps  = subchannelManager().getAllHasSteps();
    int curSubPos;
    QBitArray isPlaying(64);
    isPlaying = subchannelManager().getPlayingSubchannels();


    for( int cnt = 0; cnt < 64; cnt++ )
    {
        curSubPos = settings().getSubchannelPos(cnt);

        /** refresh sub tiles **/
        if( hasSample.at(cnt) )
        {
            m_pad[curSubPos]->setHasSample();
        }
        else
        {
            m_pad[curSubPos]->clearHasSample();
        }

        if( hasSteps.at(cnt) )
        {
            m_pad[curSubPos]->setHasSteps();
        }
        else
        {
            m_pad[curSubPos]->clearHasSteps();
        }

        /** refresh sample name **/
        QSharedPointer<Sample> samplePtr = subchannelManager().getSharedPointerToSample(cnt);
        if(samplePtr != NULL)
        {
            m_pad[settings().getSubchannelPos(cnt)]->setSampleParameters( samplePtr->getSampleStructPointer()->name,
                                                                          samplePtr->getSampleStructPointer()->lengthInMs);
        }

        /** refresh sample volume **/
        m_pad[curSubPos]->setSampleVolume();

        if( isPlaying.at(cnt) )
        {
            m_pad[curSubPos]->setIsPlaying();
        }
        else
        {
            m_pad[curSubPos]->clearIsPlaying();
        }

    }

    /** subchannel selection pads **/
    int newSubchannel = subchannelManager().getCurrentSubchannelSelection();
    m_pad[settings().getSubchannelPos(m_lastSelectedSubchannel)]->setPadToDeselectionColor();
    m_pad[settings().getSubchannelPos(newSubchannel)]->setPadToSelectionColor();
    m_lastSelectedSubchannel = newSubchannel;

    /** channel selection frame **/
    int pos = settings().getChannelPos(subchannelManager().getCurrentChannelSelection());
    int xPos = (pos % 4) * BTN_GRID * 2;
    int yPos = (pos / 4) * BTN_GRID * 2;
    m_channelSelectionFrame->move(xPos, yPos);

    m_muteAndSolo->refresh();

    this->update();
#endif
}




void PageSelection::refresh()
{
#ifndef TIMER
    qDebug() <<Q_FUNC_INFO;

    /** has tiles **/
    QBitArray hasSample = subchannelManager().hasSample();
    QBitArray hasSteps  = subchannelManager().getAllHasSteps();

    for(int i = 0; i < 64; i++)
    {
        if( hasSample.at(i) )
        {
            m_pad[settings().getSubchannelPos(i)]->setHasSample();
        }
        else
            m_pad[settings().getSubchannelPos(i)]->clearHasSample();

        if( hasSteps.at(i) )
            m_pad[settings().getSubchannelPos(i)]->setHasSteps();
        else
            m_pad[settings().getSubchannelPos(i)]->clearHasSteps();
    }


    refreshSampleNames();
    refreshSampleVolume();


    /** subchannel selection pads **/
    int newSubchannel = subchannelManager().getCurrentSubchannelSelection();
    m_pad[settings().getSubchannelPos(m_lastSelectedSubchannel)]->setPadToDeselectionColor();
    m_pad[settings().getSubchannelPos(newSubchannel)]->setPadToSelectionColor();
    m_lastSelectedSubchannel = newSubchannel;

    /** channel selection frame **/
    int pos = settings().getChannelPos(subchannelManager().getCurrentChannelSelection());
    int xPos = (pos % 4) * BTN_GRID * 2;
    int yPos = (pos / 4) * BTN_GRID * 2;
    m_channelSelectionFrame->move(xPos, yPos);

    this->update();
#endif
}

void PageSelection::refreshTiles()
{
#ifndef TIMER
    //qDebug() <<Q_FUNC_INFO;
    QBitArray isPlaying(64);
    isPlaying = subchannelManager().getPlayingSubchannels();

    for(int i = 0; i < 64; i++)
    {
        if( isPlaying.at(i) )
            m_pad[settings().getSubchannelPos(i)]->setIsPlaying();
        else
            m_pad[settings().getSubchannelPos(i)]->clearIsPlaying();
    }
#endif
}

void PageSelection::refreshMutePads()
{
#ifndef TIMER
    m_muteAndSolo->refresh();
#endif
}



void PageSelection::refreshSampleNames()
{
#ifndef TIMER
    qDebug() <<Q_FUNC_INFO;
    for( int cnt = 0; cnt < 63; cnt++ )
    {
        QSharedPointer<Sample> samplePtr = subchannelManager().getSharedPointerToSample(cnt);

        if(samplePtr != NULL)
        {
            m_pad[settings().getSubchannelPos(cnt)]->setSampleParameters( samplePtr->getSampleStructPointer()->name,
                                                                          samplePtr->getSampleStructPointer()->lengthInMs);
        }
    }
#endif
}



void PageSelection::refreshSampleVolume()
{
#ifndef TIMER
    qDebug() <<Q_FUNC_INFO;

    for( int cnt = 0; cnt < 64; cnt++ )
    {
        m_pad[settings().getSubchannelPos(cnt)]->setSampleVolume();
    }
#endif
}



void PageSelection::m_slot_selectionChanged(int id)
{
    //qDebug() <<Q_FUNC_INFO <<id;
    emit signal_subchannelSelectionPadPressed(id);
}






void PageSelection::slot_setExclusiveSubchannelSelection(int id)
{
    m_pad[m_lastSelectedSubchannel]->setPadToDeselectionColor();
    m_pad[id]->setPadToSelectionColor();
}


/** this is just an ugly hack!! **/
void PageSelection::toggleMuteAndSolo()
{
    m_muteAndSoloVisible = !m_muteAndSoloVisible;
    qDebug() <<Q_FUNC_INFO <<"m_muteAndSoloVisible" <<m_muteAndSoloVisible;

    if( m_muteAndSoloVisible )
    {
        m_muteAndSolo->setFixedSize(settings().getScreenSize().height(), settings().getScreenSize().height());
        m_muteAndSolo->refresh();
    }
    else
        m_muteAndSolo->setFixedSize(1,1);
    this->update();
}



void PageSelection::paintEvent(QPaintEvent *event)
{
    //qDebug() <<Q_FUNC_INFO;
    Q_UNUSED(event);
    QStyleOption myOption;
    myOption.initFrom(this);
    QPainter myPainter(this);
    style()->drawPrimitive( QStyle::PE_Widget, &myOption, &myPainter, this);
}
