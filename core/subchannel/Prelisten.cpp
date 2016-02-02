#include "Prelisten.h"
#include <QDebug>

#include "core/subchannel/Envelope.h"
#include "SubchannelManager.h"


Prelisten::Prelisten()
    : m_playNewSample(false)
    , m_finishedPlaying(true)
    , m_frameCounter(0)
    , m_frameNumbers(0)
    , m_sampleBuffer(nullptr)
{
}

Prelisten::~Prelisten()
{

}

TwoChannelFrame_t Prelisten::getFrame(void)
{
    TwoChannelFrame_t retVal = {0.0,0.0};

    if( m_playNewSample )
    {
        m_finishedPlaying = false;
        m_frameCounter  = 0;
        m_frameNumbers  = m_sample->getSampleStructPointer()->sndInfo.frames;
        m_sampleBuffer  = m_sample->getSampleStructPointer()->frameBuffer;
        m_channels      = m_sample->getSampleStructPointer()->sndInfo.channels;
        m_playNewSample = false;
        envelope_t envelope = subchannelManager().getCurrentEnvelope();
        m_frameCounter = envelope.start * m_frameNumbers;
        m_envEnd       = envelope.end   * m_frameNumbers;
    }
    else
    {
        if( m_finishedPlaying )
            return retVal;
    }

    if( m_channels == 1 )
    {
        retVal.left  = m_sampleBuffer[m_frameCounter];
        retVal.right = m_sampleBuffer[m_frameCounter];
        m_frameCounter++;
    }
    else // 2
    {
        retVal.left  = m_sampleBuffer[m_frameCounter];
        m_frameCounter++;
        retVal.right = m_sampleBuffer[m_frameCounter];
        m_frameCounter++;
    }

    if( m_frameCounter >= (m_envEnd) )
    {
        m_finishedPlaying = true;
        delete m_sample;
        //qDebug() <<Q_FUNC_INFO <<"Sample pointer cleared.";
    }

    return retVal;
}



void Prelisten::playSample(QString pathAndName)
{
    m_sample = new Sample(pathAndName);
    m_playNewSample = true;
    //qDebug() <<Q_FUNC_INFO <<"Sample loaded from:" <<pathAndName  <<"with" <<m_sample->getSampleStructPointer()->sndInfo.frames <<"frames.";
}

