#include "SubchannelManager.h"
#include <QDebug>
#include <Settings.h>




SubchannelManager::SubchannelManager()
    : m_currentSubchannel(0)
    , m_currentChannel(0)
{
    m_bpm = 110.0;
    m_mainVolume = 0.5;
    m_preVolume = 0.1;
    m_currentSubchannelToHeadphones = 0.0;
    m_lastMuteStates.resize(SETTINGS_NUM_OF_SUBS);

    qDebug() <<Q_FUNC_INFO <<"Init.";

    //int numOfSubs = settings().getNumberOfSubchannels();
    int numOfSubs = SETTINGS_NUM_OF_SUBS;



    for(int i = 0; i < SETTINGS_NUM_OF_SUBS; i++)
    {
        m_subchannel.append(new Subchannel(i));
    }

    m_prelisten        = new Prelisten();
    m_prelistenCurrentSubchannelSample = new Prelisten();

    qDebug() <<Q_FUNC_INFO <<"Selected subchannel:" <<m_currentSubchannel;
    qDebug() <<Q_FUNC_INFO <<numOfSubs <<"subchannels created.";
}


void SubchannelManager::init()
{

}


SubchannelManager::~SubchannelManager()
{
    qDeleteAll( m_subchannel );
    m_subchannel.clear();
    qDebug() <<"~" <<Q_FUNC_INFO;
}

float SubchannelManager::getPreVolume() const
{
    return m_preVolume;
}

void SubchannelManager::setPreVolumeRelative(float relVal)
{
    if( m_preVolume + relVal >= 1 )
    {
        m_preVolume = 1;
    }
    else if( m_preVolume + relVal <= 0 )
    {
        m_preVolume = 0;
    }
    else
    {
        m_preVolume = relVal;
    }

    qDebug() <<Q_FUNC_INFO <<"m_preVolume" <<m_preVolume;
}

void SubchannelManager::setPreVolumeAbs(float absVal)
{
    if( absVal >= 1 )
    {
        absVal  = 1;
    }
    if( absVal <= 0 )
    {
        absVal  = 0;
    }

    m_preVolume = absVal;

    qDebug() <<Q_FUNC_INFO <<"m_preVolume" <<m_preVolume;
}




void SubchannelManager::setBpm(float bpm)
{
    m_bpm = bpm;
    qDebug() <<Q_FUNC_INFO <<"Bpm set to:" <<m_bpm;
}

void SubchannelManager::setBpmRelative(float change)
{
    m_bpm = m_bpm + change;
    qDebug() <<Q_FUNC_INFO <<"Bpm changed to:" <<m_bpm;
}

float SubchannelManager::getMainVolume() const
{
    return m_mainVolume;
}

void SubchannelManager::setMainVolumeRelative(float relVal)
{
    if( (m_mainVolume + relVal) >= 1.0 )
        m_mainVolume = 1.0;
    else if( (m_mainVolume + relVal) <= 0.0  )
        m_mainVolume = 0.0;
    else
        m_mainVolume += relVal;

    qDebug() <<Q_FUNC_INFO <<"m_mainVolume:" <<m_mainVolume;
}

float SubchannelManager::getBpm() const
{
    return m_bpm;
}



void SubchannelManager::setCurrentSubchannelVolumeRelative(float vol)
{
    m_subchannel.at(m_currentSubchannel)->setVolumeRel(vol);
}

float SubchannelManager::getCurrentSubchannelVolume()
{
    return m_subchannel.at(m_currentSubchannel)->getVolume();
}



void SubchannelManager::setStepSequencerCounter(int absoluteStep)
{
   m_currentStep = absoluteStep;
   int relativeStep = m_currentStep % 64;

   for( int subch = 0; subch < SETTINGS_NUM_OF_SUBS; subch++ )
   {
       m_subchannel.at(subch)->updateRelativeStepCounter(relativeStep);
   }
   //qDebug() <<Q_FUNC_INFO <<"m_stepSequencerCounter:" <<m_currentStep;
   //qDebug() <<Q_FUNC_INFO <<"relativeStep:" <<relativeStep;
}



QBitArray SubchannelManager::hasSample()
{
    QBitArray returnVal(SETTINGS_NUM_OF_SUBS);

    for( int i = 0; i < SETTINGS_NUM_OF_SUBS; i++)
        returnVal.setBit(i, m_subchannel.at(i)->hasSample());

    return returnVal;
}



bool SubchannelManager::hasCurrentSubchannelSample()
{
    return m_subchannel.at(m_currentSubchannel)->hasSample();
}



void SubchannelManager::toggleStep(quint8 id)
{
    if (id < 64)
        m_subchannel.at(m_currentSubchannel)->toggleStep(id);
    else
        qDebug() <<Q_FUNC_INFO <<"Step id" <<id <<"not in range";
}



quint8 SubchannelManager::getStepValue(int id)
{
    return m_subchannel.at(m_currentSubchannel)->getStepValue(id);
}



void SubchannelManager::clearCurrentSubchannelPattern()
{
    m_subchannel.at(m_currentSubchannel)->clearPattern();
}



void SubchannelManager::clearCurrentChannelPattern()
{
    for(int i = 0; i < 4; i++)
        m_subchannel.at((m_currentChannel * 4) + i)->clearPattern();
}

void SubchannelManager::clearAllPattern()
{
    for( int i = 0; i < SETTINGS_NUM_OF_SUBS; i++ )
        m_subchannel.at(i)->clearPattern();
}



QBitArray SubchannelManager::getCurrentSubchannelPattern(void)
{
    return m_subchannel.at(m_currentSubchannel)->getPattern();
}

int SubchannelManager::getCurrentSubchannelStep()
{
    return m_subchannel.at(m_currentSubchannel)->getStepValue(m_currentStep);
}



QByteArray SubchannelManager::getCurrentChannelPattern(void)
{
    int subchsIdsOfCurrentChannel[4];
    int topSubchannelId;
    QBitArray  curPattern(SETTINGS_NUM_OF_SUBS);
    QByteArray retPattern;
    retPattern.resize(SETTINGS_NUM_OF_SUBS);

    for(int i = 0; i < 4; i++)
    {
        subchsIdsOfCurrentChannel[i] = (m_currentChannel * 4) + i;    //e.g. 16,17,18,19
        //qDebug() <<Q_FUNC_INFO <<"subchannel" <<i <<":" <<subchsIdsOfCurrentChannel[i];
    }

    // set all return pattern elements to 0
    for( int k = 0; k < 64; k++ )
        retPattern[k] = 0;

    // set the three background patterns
    for( int i = 0; i < 4; i++ )
    {
        if( m_currentSubchannel != subchsIdsOfCurrentChannel[i] )
        {
            curPattern = m_subchannel.at(subchsIdsOfCurrentChannel[i])->getPattern();
            //qDebug() <<Q_FUNC_INFO <<"current pattern:" <<curPattern;
            for( int j = 0; j < SETTINGS_NUM_OF_SUBS; j++)
            {
                if( curPattern.at(j) == true )
                {
                    retPattern[j] = i + 1;
                }
            }
        }
        else
        {
            topSubchannelId = i;
            //qDebug() <<Q_FUNC_INFO <<"top subchannel id:" <<topSubchannelId;
        }
    }

    // set the top pattern
    curPattern = m_subchannel.at(m_currentSubchannel)->getPattern();

    //qDebug() <<Q_FUNC_INFO <<"selected pattern:" <<curPattern;
    for ( int j = 0; j < 64; j++)
    {
        if( curPattern.at(j) )
            retPattern[j] = topSubchannelId + 1;
    }

    return retPattern;
}

int SubchannelManager::getCurrentChannelStep()
{
    qDebug() <<Q_FUNC_INFO <<"Notimplemented jet.";
    return 0;
}

int SubchannelManager::getChannelStep(int stepId)
{
    Q_UNUSED(stepId);
    qDebug() <<Q_FUNC_INFO <<"Notimplemented jet.";
    return 0;
}



QBitArray SubchannelManager::getAllHasSteps(void)
{
    QBitArray returnVal(SETTINGS_NUM_OF_SUBS);

    for( int i = 0; i < SETTINGS_NUM_OF_SUBS; i++)
    {
        returnVal.setBit(i, m_subchannel.at(i)->hasSteps());
    }

    return returnVal;
}



/** ****************************************************************************
    sample
*******************************************************************************/
TwoChannelFrame_t SubchannelManager::getFrame(int subchannelId)
{
    return m_subchannel.at(subchannelId)->getTestFrame();
}



#define NUMBER_OF_SUBCHANNELS 16
FourChannelFrame_t SubchannelManager::getFourChannelAudioFrame()
{
    FourChannelFrame_t retValue         = {0.0, 0.0, 0.0, 0.0};
    TwoChannelFrame_t  tempFrame        = {0.0, 0.0};
    TwoChannelFrame_t  collectFrames    = {0.0, 0.0};
    TwoChannelFrame_t  preSubToHpFrames = {0.0, 0.0};

    for( int subch = 0; subch < SETTINGS_NUM_OF_AUDIO_SUBS; subch++ )
    {
        tempFrame = m_subchannel.at(subch)->getFrame();
        if( m_currentSubchannel == subch )
            preSubToHpFrames = tempFrame;

        collectFrames.left  = collectFrames.left  + tempFrame.left;
        collectFrames.right = collectFrames.right + tempFrame.right;
    }

    // main output
    retValue.mainLeft  = collectFrames.left;
    retValue.mainRight = collectFrames.right;

#if 1 // return prelisten to headphones; collectFrames and tempFrames are reused here to gain better performance
    collectFrames = m_prelisten->getFrame();
    tempFrame = m_prelistenCurrentSubchannelSample->getFrame();
    retValue.preLeft  = (collectFrames.left  + (preSubToHpFrames.left  * m_currentSubchannelToHeadphones) + tempFrame.left ) * m_preVolume * m_preVolume;
    retValue.preRight = (collectFrames.right + (preSubToHpFrames.right * m_currentSubchannelToHeadphones) + tempFrame.right) * m_preVolume * m_preVolume;
#endif

#if 0 // return main to headphones
    retValue.preLeft   = collectFrames.left;
    retValue.preRight  = collectFrames.right;
#endif

    return retValue;
}


void SubchannelManager::loadSampleToCurrentSubchannel(QString pathAndName)
{
    m_subchannel.at(m_currentSubchannel)->loadSample(pathAndName);
}


void SubchannelManager::unloadSampleOfCurrentSubchannel()
{
    qDebug() <<Q_FUNC_INFO;
    m_subchannel.at(m_currentSubchannel)->unloadSample();
}


void SubchannelManager::unloadSamplesOfCurrentChannel()
{
    qDebug() <<Q_FUNC_INFO;
    m_subchannel.at(m_currentChannel * 4 + 0)->unloadSample();
    m_subchannel.at(m_currentChannel * 4 + 1)->unloadSample();
    m_subchannel.at(m_currentChannel * 4 + 2)->unloadSample();
    m_subchannel.at(m_currentChannel * 4 + 3)->unloadSample();
}


void SubchannelManager::unloadAllSamples()
{
    qDebug() <<Q_FUNC_INFO;
    for(int i = 0; i < SETTINGS_NUM_OF_SUBS; i++)
        m_subchannel.at(i)->unloadSample();
}


QBitArray SubchannelManager::getPlayingSubchannels()
{
    QBitArray isPlaying(SETTINGS_NUM_OF_SUBS);

    for( int i = 0; i < SETTINGS_NUM_OF_SUBS; i++ )
    {
        isPlaying[i] = m_subchannel.at(i)->isPlaying();
    }

    return isPlaying;
}


/** ****************************************************************************
    prelisten
*******************************************************************************/
void SubchannelManager::currentSubchannelToPrelisten(bool status)
{
    qDebug() <<Q_FUNC_INFO <<"status" <<status;
    if( status )
        m_currentSubchannelToHeadphones = 1.0;
    else
        m_currentSubchannelToHeadphones = 0.0;

    qDebug() <<Q_FUNC_INFO <<"m_currentSubchannelToHeadphones" <<m_currentSubchannelToHeadphones;
}


void SubchannelManager::loadSampleToPrelisten(QString pathAndName)
{
    qDebug() <<Q_FUNC_INFO;
    m_prelisten->playSample(pathAndName);
}


void SubchannelManager::prelistenCurrentSubchannelSample()
{
    qDebug() <<Q_FUNC_INFO;

    if( m_subchannel.at(m_currentSubchannel)->hasSample() )
    {
        QString pathAndName = m_subchannel.at(m_currentSubchannel)->getSharedSamplePtr()->getSampleStructPointer()->pathAndName;
        m_prelistenCurrentSubchannelSample->playSample(pathAndName);
    }
    else
        qDebug() <<Q_FUNC_INFO <<"Current subchannel has no sample -> nothing to prelisten to.";
}



/** ****************************************************************************
    mute
*******************************************************************************/
QBitArray SubchannelManager::getMuteStates()
{
    QBitArray muteStates(SETTINGS_NUM_OF_SUBS);

    for( int i = 0; i < SETTINGS_NUM_OF_SUBS; i++ )
    {
        muteStates[i] = m_subchannel.at(i)->isMute();
    }

    return muteStates;
}


void SubchannelManager::toggleMuteState(int id)
{
    if( (id >= 0) && (id < SETTINGS_NUM_OF_SUBS) )
        m_subchannel.at(id)->setMute( !m_subchannel.at(id)->isMute() );
    else
        qDebug() <<Q_FUNC_INFO <<"Subchannel id:" <<id <<"not in range.";
}


void SubchannelManager::unmuteAll()
{
    qDebug() <<Q_FUNC_INFO;

    if( !checkIfAllSubchannelsAreUnmuted() )
    {
        for( int i = 0; i < SETTINGS_NUM_OF_SUBS; i++ )
        {
            m_lastMuteStates[i] = m_subchannel.at(i)->isMute();
            m_subchannel.at(i)->setMute(false);
        }
        qDebug() <<Q_FUNC_INFO <<"Unmuted. Last Mute is saved.";
    }
    else
        qDebug() <<Q_FUNC_INFO <<"Nothing to unmute. Last Mute is not saved again.";
}


void SubchannelManager::lastMuteStates()
{
    qDebug() <<Q_FUNC_INFO;

    for( int i = 0; i < SETTINGS_NUM_OF_SUBS; i++ )
    {
        m_subchannel.at(i)->setMute(m_lastMuteStates[i]);
    }
}


bool SubchannelManager::checkIfAllSubchannelsAreUnmuted()
{
    bool allAreUnmute = true;

    for( int i = 0; i < SETTINGS_NUM_OF_SUBS; i++ )
    {
        if( m_subchannel.at(i)->isMute() )
        {
            allAreUnmute = false;
        }
    }
    qDebug() <<Q_FUNC_INFO <<"allAreUnmute" <<allAreUnmute;

    return allAreUnmute;
}



/** ****************************************************************************
    pan
*******************************************************************************/
void SubchannelManager::setCurrentPan(float relVal)
{
    m_subchannel.at(m_currentSubchannel)->setPan(relVal);
}


float SubchannelManager::getCurrentPan() const
{
    return m_subchannel.at(m_currentSubchannel)->getPan();
}

float SubchannelManager::getSubchannelVolume(int subchId)
{
    if(( subchId >= 0 ) && ( subchId < SETTINGS_NUM_OF_SUBS ))
        return m_subchannel.at(subchId)->getVolume();
    else
        return 0.0;
}

void SubchannelManager::togglePlayDirection()
{
    m_subchannel.at(m_currentSubchannel)->togglePlayDirection();
}


QString SubchannelManager::getCurrentSamplePathAndName()
{
    qDebug() <<Q_FUNC_INFO <<"This function is obsolete; use shared_ptr mechanism instead";
    //return m_subchannel.at(m_currentSubchannel)->getSamplePathAndName();
    return "XXX";
}




QSharedPointer<Sample> SubchannelManager::getSharedPointerToSample()
{
    return m_subchannel.at(m_currentSubchannel)->getSharedSamplePtr();
}



QSharedPointer<Sample> SubchannelManager::getSharedPointerToSample(int subchannel)
{
    if( (subchannel < 0) || (subchannel > SETTINGS_NUM_OF_SUBS) )
    {
        qDebug() <<Q_FUNC_INFO <<"Not a valide subchannel.";
    }

    return m_subchannel.at(subchannel)->getSharedSamplePtr();
}



/** ****************************************************************************
    sub/channel selection
*******************************************************************************/
void SubchannelManager::setCurrentSubchannelSelection(int id)
{
    if ((id >= 0) && (id < SETTINGS_NUM_OF_SUBS))
    {
        m_currentSubchannel = id;
        m_currentChannel = id / 4;
    }
    else
    {
        qDebug() <<Q_FUNC_INFO <<"Selected subchannel id not in range.";
    }
}


void SubchannelManager::setCurrentSubchannelSelectionRelative(int id)
{
    if ((id >= 0) && (id < 4))
    {
        m_currentSubchannel = (m_currentChannel * 4) + id;
    }
    else
    {
        qDebug() <<Q_FUNC_INFO <<"Selected subchannel id not in range.";
    }
}


void SubchannelManager::setCurrentChannelSelection(int id)
{
    if(( id >= 0 ) && (id < SETTINGS_NUM_OF_CHANNELS) )
    {
        m_currentChannel = id;
        m_currentSubchannel = id * 4;
    }
    else
    {
        qDebug() <<Q_FUNC_INFO <<"Selected Channel id not in range.";
    }
}


/** @param[in] +/- 1 */
void SubchannelManager::setCurrentChannelRelative(int change)
{
    if( change < -1)
    {
        change = -1;
        qDebug() <<Q_FUNC_INFO <<change <<"truncated to -1.";
    }
    else if (change >  1)
    {
        change =  1;
        qDebug() <<Q_FUNC_INFO <<change <<"truncated to 1.";
    }
    if( change == 0)
    {
        qDebug() <<Q_FUNC_INFO <<"Value did not change.";
        return;
    }

    setCurrentChannelSelection(m_currentChannel + change);
}



int SubchannelManager::getCurrentChannelSelection()
{
    return m_currentChannel;
}


int SubchannelManager::getCurrentSubchannelSelelectionRelative()
{
    return m_currentSubchannel % 4;
}


int SubchannelManager::getCurrentSubchannelSelection()
{
    return m_currentSubchannel;
}


QList<int> SubchannelManager::getSubchannelIdsOfCurrentChannel()
{
    QList<int> retList;

    for(int i = 0; i < 4; i++)
    {
        retList.append((m_currentChannel * 4) + i);    //e.g. 16,17,18,19
    }

    return retList;
}



/** ****************************************************************************
    Envelope
*******************************************************************************/
void SubchannelManager::setCurrentStartPointRel(float start)
{
    m_subchannel.at(m_currentSubchannel)->setStartPointRel(start);
}

void SubchannelManager::setCurrentEndPointRel(float end)
{
    m_subchannel.at(m_currentSubchannel)->setEndPointRel(end);
}

void SubchannelManager::setCurrentFadeInPointRel(float fadeIn)
{
    m_subchannel.at(m_currentSubchannel)->setFadeInPointRel(fadeIn);
}

void SubchannelManager::setCurrentFadeOutPointRel(float fadeOut)
{
    m_subchannel.at(m_currentSubchannel)->setFadeOutPointRel(fadeOut);
}

envelope_t SubchannelManager::getCurrentEnvelope()
{
    return m_subchannel.at(m_currentSubchannel)->getEnvelope();
}

envelope_t SubchannelManager::getEnvelope(int id)
{
    return m_subchannel.at(id)->getEnvelope();
}






/** ****************************************************************************
    Audio Test Functions
*******************************************************************************/
FourChannelFrame_t SubchannelManager::getFourChannelAudioTestFrame()
{
    static int m_tempCnt = 0;
    static bool m_tgl = false;

    m_tempCnt++;
    FourChannelFrame_t retValue;

    if( m_tempCnt % 50 == 0)
    {
        if( m_tgl )
            m_tgl = false;
        else
            m_tgl = true;
    }

    if( m_tgl )
    {
        retValue.mainLeft  = -0.5;
        retValue.mainRight = -0.5;
        retValue.preLeft   = -0.5;
        retValue.preRight  = -0.5;
    }
    else
    {
        retValue.mainLeft  = 0.5;
        retValue.mainRight = 0.5;
        retValue.preLeft   = 0.5;
        retValue.preRight  = 0.5;
    }

    return retValue;
}

