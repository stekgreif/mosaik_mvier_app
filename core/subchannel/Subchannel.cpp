#include "Subchannel.h"
#include <QDebug>
#include <QBitArray>


Subchannel::Subchannel(int id)
    // var for audio
    : m_audioFramesRemain(0)
    , m_audioState(0)
    , m_audioOldStep(0)
    , m_audioStepChanged(false)
    , m_pan(0.5)
    , m_audioMute(1.0)
{
    m_subchannelId = id;
    m_frameReturnState = 0;

    m_pattern = new QList<int>; /// @todo 99 - move to Pattern class

    m_envelope = new Envelope;

    for(int i = 0; i < 64; i++)
        m_pattern->append(0);

    m_hasSample = false;

    m_tempCnt = 0;
    m_tgl = false;
    m_volume = 0.6;



    m_finishedPlaying = true;
    m_step = -1;
    m_alsaStep = -1;
    m_stepHasChanged = false;
    m_frameCounter = 0;
    m_frameNumbers = 0;
    m_playForward = true ;
    m_sampleBuffer = nullptr;
}



Subchannel::~Subchannel()
{
    delete m_envelope;
    m_pattern->clear();
}

void Subchannel::setStartPointRel(float start)
{
    m_envelope->setStartPointRel(start);
}

void Subchannel::setEndPointRel(float end)
{
    m_envelope->setEndPointRel(end);
}

void Subchannel::setFadeInPointRel(float fadeIn)
{
    m_envelope->setFadeInPointRel(fadeIn);
}

void Subchannel::setFadeOutPointRel(float fadeOut)
{
    m_envelope->setFadeOutPointRel(fadeOut);
}

envelope_t Subchannel::getEnvelope()
{
    return m_envelope->getEnvelope();
}



/** sequencer **/

void Subchannel::toggleStep(int stepId)
{
    if( m_pattern->at(stepId) == 1)
    {
        m_pattern->replace(stepId, 0);
    }
    else
    {
        m_pattern->replace(stepId, 1);
    }
}


void Subchannel::clearPattern()
{
    for(int i = 0; i < 64; i++)
    {
        m_pattern->replace(i, 0);
    }
}


quint8 Subchannel::getStepValue(int stepId)
{
    return m_pattern->at(stepId);
}


QString Subchannel::getPatternForPrinting(void)
{
    QString pattern;

    for(int i = 0; i < 64; i++)
    {
        pattern += QString::number(m_pattern->at(i));
        if(((i+1) % 8) == 0)
            pattern += "  ";
    }
    return pattern;
}



QBitArray Subchannel::getPattern()
{
    QBitArray pattern(64);

    for (int i = 0; i < 64; i++)
    {
        if( m_pattern->at(i) == 1)
        {
            pattern.setBit(i);
        }
        else
        {
            pattern.clearBit(i);
        }
    }

    //qDebug() <<Q_FUNC_INFO <<pattern;
    return pattern;
}



bool Subchannel::hasSteps(void)
{
    if(m_pattern->count(1) > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}



void Subchannel::updateRelativeStepCounter(int step)
{
    m_alsaStep = step;
}



void Subchannel::setVolumeRel(float vol)
{
    if(( m_volume + vol) >= 1 )
    {
        qDebug() <<Q_FUNC_INFO <<"Vol is already at max." <<m_volume;
        m_volume = 1.0;
    }
    else if(( m_volume + vol) <= 0 )
    {
        qDebug() <<Q_FUNC_INFO <<"Vol is already at min:" <<m_volume;
        m_volume = 0.0;
    }
    else
    {
        m_volume = m_volume + vol;
        qDebug() <<Q_FUNC_INFO <<"New volume set:" <<m_volume;
    }
}



float Subchannel::getVolume()
{
    return m_volume;
}

void Subchannel::setPan(float relVal)
{
    if( (m_pan + relVal) < 0 )
        m_pan = 0;
    else if((m_pan + relVal) > 1)
        m_pan = 1;
    else
        m_pan = m_pan + relVal;

    qDebug() <<Q_FUNC_INFO <<"m_pan:" <<m_pan;
}



float Subchannel::getPan()
{
    return m_pan;
}



void Subchannel::printSubchannelPatternsToDebug(void)
{
    qDebug() <<"Subchannel" <<m_subchannelId <<getPatternForPrinting() ;
}




/** Sample **/
void Subchannel::loadSample(QString pathAndName)
{
    m_volume = 0.6;
    m_sharedSamplePtr = QSharedPointer<Sample>(new Sample(pathAndName));
    m_hasSample = true;
}


void Subchannel::unloadSample(void)
{
    qDebug() <<Q_FUNC_INFO <<"Try to unload sample of subchannel" <<m_subchannelId;

    if( m_hasSample )
    {
        m_sharedSamplePtr.clear();
        m_hasSample = false;
        m_envelope->resetAll();
        qDebug() <<Q_FUNC_INFO <<"Sample Unloaded";
    }
    else
        qDebug() <<Q_FUNC_INFO <<"Nothing to unload";
}




bool Subchannel::isPlaying()
{
    return !m_finishedPlaying;
}





bool Subchannel::isMute() const
{
    if( (m_audioMute > 0.9) && (m_audioMute < 1.1) )
        return false;
    else
        return true;
}

void Subchannel::setMute(bool state)
{
    if( state )
        m_audioMute = 0.0;
    else
        m_audioMute = 1.0;

    //qDebug() <<Q_FUNC_INFO <<"m_audioMute (0:mute):" <<m_audioMute;
}


#if 1 // 2015-05-09 mit start und endpoint
TwoChannelFrame_t Subchannel::getFrame()
{
    TwoChannelFrame_t retVal = {0.0,0.0};


    if( m_step != m_alsaStep ) // step has changed
    {
        m_step = m_alsaStep;

        if( m_pattern->at(m_step) && m_hasSample )
        {
            m_finishedPlaying = false;
            m_frameNumbers = m_sharedSamplePtr->getSampleStructPointer()->sndInfo.frames;
            m_sampleBuffer = m_sharedSamplePtr->getSampleStructPointer()->frameBuffer;
            m_channels     = m_sharedSamplePtr->getSampleStructPointer()->sndInfo.channels;

            envelope_t envelope = m_envelope->getEnvelope();

            m_frameCounter = envelope.start * m_frameNumbers;
            m_envEnd       = envelope.end   * m_frameNumbers;
        }
        else
        {
            if( m_finishedPlaying )
                return retVal;
        }
    }
    else
    {
        if( m_finishedPlaying )
            return retVal;
    }


    if( m_channels == 1 )
    {
        retVal.left  = m_sampleBuffer[m_frameCounter] * m_volume * m_volume * 2 * (1-m_pan) * m_audioMute;
        retVal.right = m_sampleBuffer[m_frameCounter] * m_volume * m_volume * 2 *    m_pan  * m_audioMute;
        m_frameCounter++;
    }
    else // 2
    {
        retVal.left  = m_sampleBuffer[m_frameCounter] * m_volume * m_volume * 2 * (1-m_pan) * m_audioMute;
        m_frameCounter++;
        retVal.right = m_sampleBuffer[m_frameCounter] * m_volume * m_volume * 2 *    m_pan  * m_audioMute;
        m_frameCounter++;
    }

    if( m_frameCounter >= (m_envEnd) )
        m_finishedPlaying = true;

    return retVal;
}
#endif


#if 0 // 2015-04-24 läuft
TwoChannelFrame_t Subchannel::getFrame()
{
    TwoChannelFrame_t retVal = {0.0,0.0};


    if( m_step != m_alsaStep ) // step has changed
    {
        m_step = m_alsaStep;

        if( m_pattern->at(m_step) && m_hasSample )
        {
            m_frameCounter = 0;
            m_finishedPlaying = false;
            m_frameNumbers = m_sharedSamplePtr->getSampleStructPointer()->info.frames;
            m_sampleBuffer = m_sharedSamplePtr->getSampleStructPointer()->buffer;
            qDebug() <<Q_FUNC_INFO <<"reloaded, m_frameNumbers:" <<m_frameNumbers;
        }
        else
        {
            if( m_finishedPlaying )
                return retVal;
        }
    }
    else
    {
        if( m_finishedPlaying )
        {
            return retVal;
        }
    }

    retVal.left  = m_sampleBuffer[m_frameCounter] * m_volume;
    retVal.right = retVal.left;
    m_frameCounter++;

    if( m_frameCounter >= (m_frameNumbers - 2) )
        m_finishedPlaying = true;

    return retVal;
}
#endif






TwoChannelFrame_t Subchannel::getTestFrameWithSteps()
{
    TwoChannelFrame_t retValue = {0.0,0.0};
    m_tempCnt++;

    if( m_tempCnt % 50 == 0)
    {
        if( m_tgl )
            m_tgl = false;
        else
            m_tgl = true;
    }
    if( m_pattern->at(m_stepCounter) )
    {
        // generate square
        if( m_tgl )
        {
            retValue.left  = -1.0;
            retValue.right = -1.0;
        }
        else
        {
            retValue.left  = 1.0;
            retValue.right = 1.0;
        }
    }
    else
    {
        retValue.left  = 0.0;
        retValue.right = 0.0;
    }

    return retValue;
}




TwoChannelFrame_t Subchannel::getTestFrame()
{
    TwoChannelFrame_t retValue = {0.0,0.0};
    m_tempCnt++;

    if( m_tempCnt % 50 == 0)
    {
        if( m_tgl )
            m_tgl = false;
        else
            m_tgl = true;
    }

    if( (m_stepCounter % 4) == 0 )
    {
        if( m_tgl )
        {
            retValue.left  = -1.0;
            retValue.right = -1.0;
        }
        else
        {
            retValue.left  = 1.0;
            retValue.right = 1.0;
        }
    }
    else
    {
        retValue.left  = 0.0;
        retValue.right = 0.0;
    }

    return retValue;
}




QSharedPointer<Sample> Subchannel::getSharedSamplePtr(void)
{
    return m_sharedSamplePtr;
}

bool Subchannel::hasSample(void)
{
    return m_hasSample;
}










