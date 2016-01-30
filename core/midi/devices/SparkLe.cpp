#include "SparkLe.h"

#include "Settings.h"
#include "MosaikTypes.h"

#include "core/midi/MidiNames.h"
#include "core/subchannel/SubchannelManager.h"

#include <QDebug>
#include <QThread>



SparkLe::SparkLe()
{
    qDebug() <<Q_FUNC_INFO <<"Get connected midi device" <<settings().getConnectedMidiDevicePort();
    m_midiOut = new MidiOut(settings().getConnectedMidiDevicePort());
    m_midiIn  = new MidiIn (settings().getConnectedMidiDevicePort());
    m_midiIn->start();
    resetHardware();

    connect(m_midiIn, SIGNAL(signal_midiMsgReceived(quint8*)), this, SLOT(slot_midiMsgReceived(quint8*)) );

    m_patternSection = 0;
    m_lastStepSequencerLed = 0;
    m_midiOut->sendData(Mosaik::MidiCommand::noteOn,  41 + m_patternSection, 1);

}

SparkLe::~SparkLe()
{
    m_midiOut->sendData(Mosaik::MidiCommand::noteOff, 24, 1);
    delete m_midiOut;
    delete m_midiIn;
    qDebug() <<"~" <<Q_FUNC_INFO;
}

void SparkLe::setStepLed(int i)
{
    quint8 data[3];
    data[0] = Mosaik::MidiCommand::noteOn;
    data[1] = i;
    data[2] = 10;
    m_midiOut->sendDataCopy(data, 3);
}

void SparkLe::sendRawData(const QByteArray &data)
{
    Q_UNUSED(data);
    qDebug() <<Q_FUNC_INFO <<"NOTHING DONE";
}

void SparkLe::setSubchannelPattern()
{
    qDebug() <<Q_FUNC_INFO;

    QBitArray pattern = subchannelManager().getCurrentSubchannelPattern();
    QByteArray midiData;
    midiData.resize(48);

    for (int i = 0; i < midiData.size()/3; i++)
    {
        midiData[3*i+1] = i;

        if( pattern.at(i + (m_patternSection * 16)) )
        {
            midiData[3*i+0] = Mosaik::MidiCommand::noteOn;
        }
        else
        {
            midiData[3*i+0] = Mosaik::MidiCommand::noteOff;
        }
        midiData[3*i+2] = 1;
    }

    m_midiOut->sendData(midiData);
    qDebug() <<Q_FUNC_INFO <<"sending finished";

}

void SparkLe::setChannelPattern()
{

}

void SparkLe::refreshSequencer()
{
    setSubchannelPattern();
}

void SparkLe::refreshSubchannelSelection()
{

}


/** @brief  The SparkLe Hardware is a bit slow and the internal buffer size is limited to ~96 elements
            To make it work midi command by midi command is sent and a delay of 200 µs is needed to give
            the SparkLe hardware some time.
*/
void SparkLe::resetHardware()
{
    for (int cnt = 0; cnt < 76; cnt++)
    {
        m_midiOut->sendData(Mosaik::MidiCommand::noteOff, cnt, 1);
        QThread::usleep(200);
    }
}

void SparkLe::setStepsequencerLed(int stepLedId)
{
    //qDebug() <<Q_FUNC_INFO;

    QBitArray pattern = subchannelManager().getCurrentSubchannelPattern();

    if( pattern.at(m_lastStepSequencerLed) )
        m_midiOut->sendData(Mosaik::MidiCommand::noteOn, m_lastStepSequencerLed, 1);
    else
        m_midiOut->sendData(Mosaik::MidiCommand::noteOff, m_lastStepSequencerLed, 1);


    m_lastStepSequencerLed = stepLedId % 16;

    // invert step
    if( pattern.at(m_lastStepSequencerLed) )
        m_midiOut->sendData(Mosaik::MidiCommand::noteOff, m_lastStepSequencerLed, 1);
    else
        m_midiOut->sendData(Mosaik::MidiCommand::noteOn, m_lastStepSequencerLed, 1);

}



void SparkLe::setMainVolume(quint8 volume)
{
    Q_UNUSED(volume);
}


void SparkLe::slot_midiMsgReceived(quint8* data)
{
    switch (data[0])
    {
        case Mosaik::MidiCommand::noteOn:
            midiNoteOnReceived(data[1], data[2]);
            break;
        case Mosaik::MidiCommand::noteOff:
            midiNoteOffReceived(data[1], data[2]);
            break;
        case Mosaik::MidiCommand::controlChange:
            midiCcReceived(data[1], data[2]);
            break;
        default:
            break;
    }
}



void SparkLe::changePatternSectionFocus(int section)
{
    m_midiOut->sendData(Mosaik::MidiCommand::noteOff,  41 + m_patternSection, 1);

    if(section == 0) // rotate left
    {
        m_patternSection  = (m_patternSection - 1) & 0b00000011;
    }
    else // rotate right
    {
        m_patternSection  = (m_patternSection + 1) & 0b00000011;
    }

    m_midiOut->sendData(Mosaik::MidiCommand::noteOn,  41 + m_patternSection, 1);

    setSubchannelPattern();

    qDebug() <<Q_FUNC_INFO <<"m_patternSection:" <<m_patternSection;
}



void SparkLe::midiNoteOnReceived(quint8 id, quint8 value)
{
    qDebug() <<Q_FUNC_INFO <<"Id:" <<id <<"Value:" <<value;

    switch (id)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        {
            emit signal_stepButtonPressed(id + (m_patternSection * 16));
            break;
        }

        case 36:
        case 37:
        {
            changePatternSectionFocus(id-36);
            break;
        }

        default:
            break;
    }
}



void SparkLe::midiNoteOffReceived(quint8 id, quint8 value)
{
    qDebug() <<Q_FUNC_INFO <<"Id:" <<id <<"Value:" <<value;
}





void SparkLe::midiCcReceived(quint8 id, quint8 value)
{
    qDebug() <<Q_FUNC_INFO <<"Id:" <<id <<"Value:" <<value;
    switch (id)
    {
    //
        case Mosaik::SparkLeCc::Volume:  //47 end
            emit signal_mainVolume(value - 64);
            break;
        case Mosaik::SparkLeCc::Tempo: // 48: start
            emit signal_bpmChanged( (float) (value - 64) );
            break;
        case Mosaik::SparkLeCc::Divide: // 52  fi
            id = 2;
            emit signal_erpChanged(id, (value-64) * 10);
            break;
        case Mosaik::SparkLeCc::Move: //53:  fo
            id = 3;
            emit signal_erpChanged(id, (value-64) * 10);
            break;
        case Mosaik::SparkLeCc::P1:
            emit signal_currentSubchannelVol( ((float)(value - 64))/100 );
            break;
        case Mosaik::SparkLeCc::P2:
            emit signal_currentSubchannelPanRel(((float)(value - 64))/100 );
            break;
        case Mosaik::SparkLeCc::P3:
            break;
        default:
            qDebug() <<Q_FUNC_INFO <<"Not implemented yet.";
            break;
    }
}


