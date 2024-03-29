#include "MosaikMini.h"
#include "Settings.h"
#include "MosaikTypes.h"
#include "core/midi/MidiNames.h"
#include "core/subchannel/SubchannelManager.h"
#include <QObject>
#include <QDebug>

#define SAVE_BW 0

struct RgbColors
{
    enum rgbColors
    {
       off     = 0xF,
       red     = 0xD,
       green   = 0xB,
       yellow  = 0x9,
       blue    = 0x7,
       magenta = 0x5,
       cyan    = 0x3,
       white   = 0x1
    };
};


MosaikMini::MosaikMini()
{
    m_singlePatternView = false;
    m_lastStepSequencerLed = 0;
    m_shiftBpm = false;
    m_shiftMainVol = false;
    m_shiftPan = false;
    m_shiftPitch = false;
    m_shiftPlayDir = false;
    m_shiftPan = false;
    m_tglSubToPre = false;
    m_mute = false;

    qDebug() <<Q_FUNC_INFO <<"Get connected midi device" <<settings().getConnectedMidiDevicePort();
    m_midiOut = new MidiOut(settings().getConnectedMidiDevicePort());
    m_midiIn  = new MidiIn (settings().getConnectedMidiDevicePort());
    connect(m_midiIn, SIGNAL(signal_midiMsgReceived(quint8*)), this, SLOT(slot_midiMsgReceived(quint8*)) );
    m_midiIn->start();

    resetHardware();
    refreshSubchannelSelection();
}


MosaikMini::~MosaikMini()
{
    resetHardware();
    delete m_midiOut;
    delete m_midiIn;
    qDebug() <<"~" <<Q_FUNC_INFO;
}


void MosaikMini::setLedState(int section, int id, bool state)
{
    qDebug() <<Q_FUNC_INFO <<section <<id <<state;
    switch( section )
    {
        case MosaikMiniDevice::MidiChannelMen:
        {
            if( (id >= 0) && (id < 8) )
            {
                qDebug() <<Q_FUNC_INFO <<"toggle ";
                m_midiOut->sendData(Mosaik::MidiCommand::noteOn | Mosaik::MidiChannels::Men, id, state);
            }
            break;
        }
    }
}


void MosaikMini::setStepLed(int i)
{
    quint8 data[3];
    data[0] = MIDI_MSG_NOTE_ON | MIDI_CH_SEQ;
    data[1] = i;
    data[2] = 1;
    m_midiOut->sendDataCopy(data, 3);
}


void MosaikMini::sendRawData(const QByteArray &data)
{
    m_midiOut->sendData(data);
}


#if !SAVE_BW // original working function
void MosaikMini::setSubchannelPattern(void)
{
    qDebug() <<Q_FUNC_INFO;

    QBitArray pattern = subchannelManager().getCurrentSubchannelPattern();
    int currentRelativeSubchannel = subchannelManager().getCurrentSubchannelSelectionRelative();

    QByteArray midiData;
    midiData.resize(192);

    //qDebug() <<Q_FUNC_INFO <<"midi bytes to send:" <<midiData.size();

    //for (int i = 0; i < midiData.size()/3; i++)
    for (int i = 0; i < midiData.size()/3; i++)
    {
        midiData[3*i+0] = (char) (MIDI_MSG_NOTE_ON | MIDI_CH_SEQ);
        midiData[3*i+1] = i;

        if( pattern.at(i) )
        {
            switch (currentRelativeSubchannel)
            {
                case 0:
                    midiData[3*i+2] = RgbColors::red;
                    break;
                case 1:
                    midiData[3*i+2] = RgbColors::blue;
                    break;
                case 2:
                    midiData[3*i+2] = RgbColors::yellow;
                    break;
                case 3:
                    midiData[3*i+2] = RgbColors::magenta;
                    break;
                default:
                    break;
            }
        }
        else
        {
            midiData[3*i+2] = RgbColors::off;
        }
    }
    m_midiOut->sendData(midiData);
    //qDebug() <<Q_FUNC_INFO <<"Midi Bytes to send:" <<midiData.size();
}
#endif


#if SAVE_BW // 2016-02-21 bandwidth saving function
void MosaikMini::setSubchannelPattern(void)
{
    //qDebug() <<Q_FUNC_INFO;

    QBitArray pattern = subchannelManager().getCurrentSubchannelPattern();
    QByteArray midiData;
    midiData.resize(195);

    midiData[0] = MIDI_MSG_NOTE_ON | MIDI_CH_SEQ;
    midiData[1] = 127;                              // message to all leds
    midiData[2] = MosaikMiniDevice::RgbColorOff;    // all off

    int bufferSizeCnt = 3;
    int color = subchannelManager().getCurrentSubchannelSelelectionRelative();

    for (int step = 0; step < 64; step++)
    {
        if( pattern.at(step) )
        {
            midiData[ bufferSizeCnt + 0 ] = MIDI_MSG_NOTE_ON | MIDI_CH_SEQ;
            midiData[ bufferSizeCnt + 1 ] = step;
            midiData[ bufferSizeCnt + 2 ] = color;
            bufferSizeCnt += 3;
        }
    }
    midiData.resize(bufferSizeCnt);
    m_midiOut->sendData(midiData);
    qDebug() <<Q_FUNC_INFO <<"Midi Bytes to send:" <<bufferSizeCnt;
}
#endif


#if !SAVE_BW // original working function
void MosaikMini::setChannelPattern(void)
{
    qDebug() <<Q_FUNC_INFO;

    QByteArray pattern = subchannelManager().getCurrentChannelPattern();
    QByteArray midiData;
    midiData.resize(192);

    for (int cnt = 0; cnt < 64; cnt++)
    {
        midiData[3*cnt+0] = (char) (MIDI_MSG_NOTE_ON | MIDI_CH_SEQ);
        midiData[3*cnt+1] = cnt;

        switch (pattern.at(cnt))
        {
            case 0:
                midiData[3*cnt+2] = MosaikMiniDevice::RgbColorOff;
                break;
            case 1:
                midiData[3*cnt+2] = MosaikMiniDevice::RgbColorRed;
                break;
            case 2:
                midiData[3*cnt+2] = MosaikMiniDevice::RgbColorblue;
                break;
            case 3:
                midiData[3*cnt+2] = MosaikMiniDevice::RgbColoryellow;
                break;
            case 4:
                midiData[3*cnt+2] = MosaikMiniDevice::RgbColormagenta;
                break;
            default:
                midiData[3*cnt+2] = MosaikMiniDevice::RgbColorOff;
                break;
        }
    }
    m_midiOut->sendData(midiData);
    //qDebug() <<Q_FUNC_INFO <<"Midi Bytes to send:" <<midiData.size();
}
#endif


#if SAVE_BW // 2016-02-21 bandwidth saving function
void MosaikMini::setChannelPattern(void)
{
    qDebug() <<Q_FUNC_INFO;

    QByteArray pattern = subchannelManager().getCurrentChannelPattern();
    QByteArray midiData;
    midiData.resize(195);

    midiData[0] = MIDI_MSG_NOTE_ON | MIDI_CH_SEQ;
    midiData[1] = 127;  // message to all leds
    midiData[2] = MosaikMiniDevice::RgbColorOff;  // all off

    int bufferSizeCnt = 3;
    for (int cnt = 0; cnt < 64; cnt++)
    {
        if( pattern.at(cnt) )
        {
            midiData[ bufferSizeCnt + 0 ] = MIDI_MSG_NOTE_ON | MIDI_CH_SEQ;
            midiData[ bufferSizeCnt + 1 ] = cnt;
            midiData[ bufferSizeCnt + 2 ] = getColorValue(pattern.at(cnt));
            bufferSizeCnt += 3;
        }
    }

    midiData.resize(bufferSizeCnt);
    m_midiOut->sendData(midiData);
    qDebug() <<Q_FUNC_INFO <<"Midi Bytes to send:" <<bufferSizeCnt;
}
#endif


void MosaikMini::refreshSequencer()
{
    qDebug() <<Q_FUNC_INFO;
    if( m_singlePatternView == true )
    {
        setSubchannelPattern();
    }
    else
    {
        setChannelPattern();
    }
}


void MosaikMini::resetHardware()
{
    // all leds off
    //qDebug() <<Q_FUNC_INFO <<"Hardware reseted.";
    quint8 data[3];
    data[0] = MIDI_MSG_NOTE_ON | MIDI_CH_SYS;
    data[1] = 127;
    data[2] = 0;
    m_midiOut->sendDataCopy(data, 3);

    refreshSubchannelSelection();
}


void MosaikMini::toggleSinglePatternView(void)
{
    qDebug() <<Q_FUNC_INFO;
    quint8 data[3];
    data[0] = Mosaik::MidiCommand::noteOn | Mosaik::MidiChannels::Fnl;
    data[1] = 10;

    if( m_singlePatternView == true )
    {
        m_singlePatternView = false;
        data[2] = 0;
    }
    else
    {
        m_singlePatternView = true;
        data[2] = 1;
    }

    m_midiOut->sendDataCopy(data, 3);

    //qDebug() <<Q_FUNC_INFO <<"m_singlePatternView:" <<m_singlePatternView;
}


void MosaikMini::refreshSubchannelSelection(void)
{
    qDebug() <<Q_FUNC_INFO;
    QByteArray midiData;
    midiData.resize(12);

    for( int i = 0; i < 4; i++)
    {
        midiData[3*i+0] = (char) (Mosaik::MidiCommand::noteOn | Mosaik::MidiChannels::Fnl);
        midiData[3*i+1] = 12 + i; // led offset
        midiData[3*i+2] = 0;
    }

    int led = subchannelManager().getCurrentSubchannelSelectionRelative();
    midiData[3*led +2] = 1;

    m_midiOut->sendData(midiData);
}



int MosaikMini::getColorValue(int id)
{
    switch (id)
    {
        case 0:
            return MosaikMiniDevice::RgbColorOff;
        case 1:
            return MosaikMiniDevice::RgbColorRed;
        case 2:
            return MosaikMiniDevice::RgbColorblue;
        case 3:
            return MosaikMiniDevice::RgbColoryellow;
        case 4:
            return MosaikMiniDevice::RgbColormagenta;
        default:
            return MosaikMiniDevice::RgbColorOff;
    }
}



void MosaikMini::setStepsequencerLed(int stepLedId)
{
    //qDebug() <<Q_FUNC_INFO;

    if( m_singlePatternView )
    {
        QBitArray pattern = subchannelManager().getCurrentSubchannelPattern();
        //qDebug() <<Q_FUNC_INFO <<"QBitArray pattern:" <<pattern.size();
        int color;

        if( pattern.at(m_lastStepSequencerLed) )
        {
            //qDebug() <<Q_FUNC_INFO <<"if";
            color = getColorValue(subchannelManager().getCurrentSubchannelSelectionRelative() + 1);
        }
        else
        {
            //qDebug() <<Q_FUNC_INFO <<"else";
            color = getColorValue( 0 );
        }

        m_midiOut->sendData(Mosaik::MidiCommand::noteOn | Mosaik::MidiChannels::Seq, m_lastStepSequencerLed, color);
    }
    else
    {
        QByteArray pattern = subchannelManager().getCurrentChannelPattern();
        //qDebug() <<Q_FUNC_INFO <<"QByteArray pattern:" <<pattern.size();
        int topLayerStep = pattern.at(m_lastStepSequencerLed);
        m_midiOut->sendData(Mosaik::MidiCommand::noteOn | Mosaik::MidiChannels::Seq, m_lastStepSequencerLed, getColorValue(topLayerStep));
    }

    m_midiOut->sendData(Mosaik::MidiCommand::noteOn | Mosaik::MidiChannels::Seq, stepLedId, RgbColors::white);
    m_lastStepSequencerLed = stepLedId;
}



/** ****************************************************************************
    MIDI IN
*******************************************************************************/
void MosaikMini::slot_midiMsgReceived(quint8* data)
{
    qDebug() <<Q_FUNC_INFO;

    quint8 midiBuffer[3] = {};

    midiBuffer[0] = data[0];    // midi ch, status
    midiBuffer[1] = data[1];    // note/id
    midiBuffer[2] = data[2];    // value
    //qDebug() <<Q_FUNC_INFO <<"xxxxxxxx" <<data[0] <<data[1] <<data[2];

    switch (midiBuffer[0] & 0b00001111)
    {
        /** Sequencer **/
        case MIDI_CH_SEQ:
        {
            //qDebug() <<"SEQ" <<data[1] <<data[2];
            if( data[0] == Mosaik::MidiCommand::noteOn )
            {
                emit signal_stepButtonPressed(data[1]);
            }
            emit signal_seqMsg(data[1], data[2]);
            break;
        }

        /** Function Left **/
        case MIDI_CH_FNL:
        {
            //qDebug() <<"FNL" <<data[0] <<data[1] <<data[2];

            //if( (data[0] & 0xF0) == Mosaik::MidiCommand::noteOn )
            {
                switch (data[1])
                {
                    case 0:
                        if( data[0] == (Mosaik::MidiCommand::noteOn | Mosaik::MidiChannels::Fnl) )
                        {
                            emit signal_functionLeftButton00Pressed();
                        }
                        break;
                    case 1:
                        if( data[0] == (Mosaik::MidiCommand::noteOn | Mosaik::MidiChannels::Fnl) )
                        {
                            emit signal_functionLeftButton01Pressed();
                        }
                        break;
                    case 2:
                        emit signal_functionLeftButton02Pressed(); break;
                    case 3:
                        if( data[0] == (Mosaik::MidiCommand::noteOn | Mosaik::MidiChannels::Fnl) )
                        {
                            emit signal_functionLeftButton03Pressed();
                        }
                        break;
                    case 4:
                        emit signal_functionLeftButton04Pressed();
                        break;
                    case 5:
                        {
                            emit signal_functionLeftButton05Pressed();
                        }break;
                    case 6:
                        if( data[0] == (Mosaik::MidiCommand::noteOn | Mosaik::MidiChannels::Fnl) )
                        {
                            emit signal_lastMute();
                            qDebug() <<Q_FUNC_INFO <<"LAST MUTE";
                        }
                        break;
                    case 7:
                        if( data[0] == (Mosaik::MidiCommand::noteOn | Mosaik::MidiChannels::Fnl) )
                        {
                            emit signal_unmuteAll();
                            qDebug() <<Q_FUNC_INFO <<"UNMUTE ALL";
                        }
                        break;
                    case 8:
                        if( data[0] == (Mosaik::MidiCommand::noteOn | Mosaik::MidiChannels::Fnl) )
                        {
                            m_mute = !m_mute;
                            emit signal_muteAndSolo(m_mute);
                        }
                        break;
                    case 9:
                        if( data[0] == (Mosaik::MidiCommand::noteOn | Mosaik::MidiChannels::Fnl) )
                        {
                            emit signal_muteAndSolo(true);
                        }
                        else if( data[0] == (Mosaik::MidiCommand::noteOff | Mosaik::MidiChannels::Fnl) )
                        {
                            emit signal_muteAndSolo(false);
                        }
                        break;
                    case 10:
                        if( data[0] == (Mosaik::MidiCommand::noteOn | Mosaik::MidiChannels::Fnl) )
                        {
                            toggleSinglePatternView();
                            refreshSequencer();
                        }
                        break;
                    case 11:
                        if( data[0] == (Mosaik::MidiCommand::noteOn | Mosaik::MidiChannels::Fnl) )
                        {
                            emit signal_functionLeftButton11Pressed();
                        }
                        break;
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                        if( data[0] == (Mosaik::MidiCommand::noteOn | Mosaik::MidiChannels::Fnl) )
                        {
                            emit signal_functionSelectSubchannelRelative(data[1] - 12);
                            emit signal_prelistenSubchannelSample();
                        }
                        break;
                    default:
                        break;
                }
            }
            break;
        }

        /** Function Right **/
        case MIDI_CH_FNR:
        {
            //qDebug() <<Q_FUNC_INFO <<"FNR";
            switch (data[1])
            {
                case 0:
                    if( data[0] == (Mosaik::MidiChannels::Fnr | Mosaik::MidiCommand::noteOn) )
                    {
                        m_shiftMainVol = true;
                    }
                    else if( data[0] == (Mosaik::MidiChannels::Fnr | Mosaik::MidiCommand::noteOff) )
                    {
                        m_shiftMainVol = false;
                    }
                    break;
                case 1:
                    if( data[0] == (Mosaik::MidiChannels::Fnr | Mosaik::MidiCommand::noteOn) )
                    {
                        m_shiftBpm = true;
                    }
                    else if( data[0] == (Mosaik::MidiChannels::Fnr | Mosaik::MidiCommand::noteOff) )
                    {
                        m_shiftBpm = false;
                    }
                    break;
                case 2: //play
                    if( data[0] == (Mosaik::MidiChannels::Fnr | Mosaik::MidiCommand::noteOn)  )
                    {
                        if( m_shiftPlayDir )
                        {
                            emit signal_playDirection(true);
                            m_shiftPlayDir = false;
                        }
                        else
                        {
                            emit signal_playDirection(false);
                            m_shiftPlayDir = true;
                        }
                    }
                    break;
                case 3: //pitch
                    if( data[0] == (Mosaik::MidiChannels::Fnr | Mosaik::MidiCommand::noteOn) )
                    {
                        m_shiftPitch   = !m_shiftPitch;
                        m_shiftPlayDir = false;
                        m_shiftPan     = false;
                        //qDebug() <<Q_FUNC_INFO <<"PlayDir:" <<m_shiftPlayDir <<" -- Pitch:" <<m_shiftPitch <<" -- Pan:" <<m_shiftPan;
                    }
                    break;
                case 4: //pan
                    if( data[0] == (Mosaik::MidiChannels::Fnr | Mosaik::MidiCommand::noteOn) )
                    {
                        m_shiftPan     = !m_shiftPan;
                        m_shiftPlayDir = false;
                        m_shiftPitch   = false;
                        //qDebug() <<Q_FUNC_INFO <<"PlayDir:" <<m_shiftPlayDir <<" -- Pitch:" <<m_shiftPitch <<" -- Pan:" <<m_shiftPan;
                    }
                    break;
                case 5:
                    emit signal_functionRightButton05Pressed(); break;
                case 6:
                    emit signal_functionRightButton06Pressed(); break;
                case 7:
                    if( data[0] == (Mosaik::MidiChannels::Fnr | Mosaik::MidiCommand::noteOn) )
                    {
                        emit signal_setPathId(8);
                    }
                    break;
                case  8:
                case  9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                    if( data[0] == (Mosaik::MidiChannels::Fnr | Mosaik::MidiCommand::noteOn) )
                    {
                        emit signal_setPathId( data[1] - 8);
                    }
                    break;
                default:
                    break;
            }
            break;
        }
        /** Menu **/
        //  0   1   2
        //  3   4   5
        //  6   7   8
        case MIDI_CH_MEN:
        {
            //qDebug() <<Q_FUNC_INFO <<"MEN" <<data[0] <<data[1] <<data[2];
            emit signal_menMsg(data[1], data[2]);

            switch (data[1])
            {
                case 0: // prelisten sequence
                {
                    if( data[0] == (Mosaik::MidiChannels::Men | Mosaik::MidiCommand::noteOn) )
                    {
                        if( m_tglSubToPre )
                        {
                            emit signal_currentSubchannelToPre(false);
                            m_tglSubToPre = false;
                        }
                        else
                        {
                            emit signal_currentSubchannelToPre(true);
                            m_tglSubToPre = true;
                        }
                    }
                    break;
                }
                case 1:
                {
                    if( data[0] == (Mosaik::MidiChannels::Men | Mosaik::MidiCommand::noteOn) )
                    {
                        emit signal_loadSample();
                    }
                    break;
                }
                case 2:
                {
                    if( data[0] == (Mosaik::MidiChannels::Men | Mosaik::MidiCommand::noteOn) )
                    {               
                        // up:
                        emit signal_menuNavigation(1);
                        emit signal_prelistenBrowserSample();
                    }
                    break;
                }
                case 3: // close folder
                {
                    if( data[0] == (Mosaik::MidiChannels::Men | Mosaik::MidiCommand::noteOn) )
                    {
                        emit signal_browserCloseFolder();
                    }
                    break;
                }
                case 4:
                {
                    if( data[0] == (Mosaik::MidiChannels::Men | Mosaik::MidiCommand::noteOn) )
                    {
                        // prelisten browser
                        emit signal_prelistenBrowserSample();
                    }
                    break;
                }
                case 5:
                {
                    if( data[0] == (Mosaik::MidiChannels::Men | Mosaik::MidiCommand::noteOn) )
                    {
                        // prelisten pad sample
                        emit signal_prelistenSubchannelSample();
                    }
                    break;
                }
                case 6:
                {
                    if( data[0] == (Mosaik::MidiChannels::Men | Mosaik::MidiCommand::noteOn) )
                    {
                        // net pad
                        emit signal_selectionSubchannelNext();
                    }
                    break;
                }
                case 7:
                {
                    if( data[0] == (Mosaik::MidiChannels::Men | Mosaik::MidiCommand::noteOn) )
                    {
                        // down
                        emit signal_menuNavigation(-1);
                        emit signal_prelistenBrowserSample();
                    }
                    break;
                }
                case 8: // encoder pushed
                {
                    if( data[0] == (Mosaik::MidiChannels::Men | Mosaik::MidiCommand::noteOn) )
                    {
                        // emit signal_browserOpenFolder();
                        emit signal_menuEncoderPushed();
                    }
                    break;
                }
                case 9: // encoder rotated
                {
                    if( m_shiftBpm )
                    {
                        emit signal_bpmChanged( (float) (data[2] - 64) );
                    }
                    else if( m_shiftMainVol )
                    {
                        emit signal_mainVolume( ((float) (data[2] - 64)) / 100 );
                    }
                    else if( m_shiftPan )
                    {
                        emit signal_currentPan(((float) (data[2] - 64)) / 100 );
                    }
                    else
                    {
                        emit signal_encChanged(-1*(data[2] - 64));
                        qDebug() <<Q_FUNC_INFO <<"encoder rotated";
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case MIDI_CH_ERP:
        {
            qDebug() <<"ERP" <<data[1] <<data[2];
            emit signal_erpMsg(data[1], data[2]);
            qint8 diff = data[2] - 64;
            emit signal_erpChanged(data[1], diff);
            break;
        }
        case MIDI_CH_PAD:
        {
            qDebug() <<"PAD" <<data[1] <<data[2];
            emit signal_padMsg(data[1], data[2]);
            break;
        }
        case MIDI_CH_CRF:
        {
            qDebug() <<"CRF" <<data[1] <<data[2];
            break;
        }
        case MIDI_CH_AHO:
        {
            qDebug() <<"AHO" <<data[1] <<data[2];
            emit signal_ahoMsg(data[1], data[2]);
            break;
        }
        case MIDI_CH_AHC:
        {
            switch (data[1])
            {
                case 0:
                {
                    qDebug() <<"APA" <<data[1] <<data[2];
                    emit signal_mainVolume( (float)data[2] / 127.0 );
                    break;
                }
                case 1:
                {
                    qDebug() <<"AHC" <<data[1] <<data[2];
                    emit signal_headphoneVolume( (float)data[2] / 127.0 );
                    break;
                }
            }
            break;
        }
        case MIDI_CH_APA: // no hardware for this
        {
            qDebug() <<"APA" <<data[1] <<data[2];
            //emit signal_apaMsg(data[1], data[2]);
            emit signal_mainVolume( (float)data[2] / 127 );
            break;
        }
        case MIDI_CH_AIN:
        {
            qDebug() <<"AIN" <<data[1] <<data[2];
            emit signal_ainMsg(data[1], data[2]);
            break;
        }
        case MIDI_CH_SPR:
        {
            qDebug() <<"SPR" <<data[1] <<data[2];
            emit signal_sprMsg(data[1], data[2]);
            break;
        }
        case MIDI_CH_SYS:
        {
            qDebug() <<"SYS" <<data[1] <<data[2];
            emit signal_sysMsg(data[1], data[2]);
            break;
        }
        default:
        {
            qDebug() <<"not supported" <<data[1] <<data[2];
            break;
        }
    }
}


/** @param volume: 0->no attenuation; 127->mute */
void MosaikMini::setMainVolume(quint8 volume)
{
    if( volume > 127)
    {
        volume = 127;
    }
    //qDebug() <<Q_FUNC_INFO <<volume;

    m_midiOut->sendData(Mosaik::MidiCommand::controlChange | Mosaik::MidiChannels::Apa, 0, volume);
}


void MosaikMini::subToPreLed(bool state)
{
    if( state )
    {
        m_midiOut->sendData(Mosaik::MidiCommand::noteOn | Mosaik::MidiChannels::Men, 0, 1);
    }
    else
    {
        m_midiOut->sendData(Mosaik::MidiCommand::noteOn | Mosaik::MidiChannels::Men, 0, 0);
    }
}




