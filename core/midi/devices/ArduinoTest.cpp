#include "ArduinoTest.h"

#include "MosaikMini.h"
#include "core/midi/MidiNames.h"
#include "Settings.h"
#include "MosaikTypes.h"
#include "core/midi/MidiNames.h"
#include "core/subchannel/SubchannelManager.h"

#include <QObject>
#include "QDebug"

//#define MIDI_TO_ARDUINO
#define MIDI_FROM_ARDUINO



ArduinoTest::ArduinoTest()
{
#if MIDI_TO_ARDUINO
    m_lastStepSequencerLed = 0;
    qDebug() <<Q_FUNC_INFO <<"Get connected midi device" <<settings().getConnectedMidiDevicePort();
    m_midiOut = new MidiOut(settings().getConnectedMidiDevicePort());
#endif

    m_midiIn  = new MidiIn (settings().getConnectedMidiDevicePort());
    connect(m_midiIn, SIGNAL(signal_midiMsgReceived(quint8*)), this, SLOT(slot_midiMsgReceived(quint8*)) );
    m_midiIn->start();

}

ArduinoTest::~ArduinoTest()
{

}



void ArduinoTest::setStepLed(int i)
{
#if MIDI_TO_ARDUINO
    qDebug() <<Q_FUNC_INFO <<"send midi";
    quint8 data[3];
    data[0] = MIDI_MSG_CONTROL_CHANGE | 2;  // 0 = red
    data[1] = i;    // led
    data[2] = 55;  // brightness
    m_midiOut->sendDataCopy(data, 3);
#endif
}

void ArduinoTest::sendRawData(const QByteArray &data)
{

}

void ArduinoTest::setSubchannelPattern()
{

}

void ArduinoTest::setChannelPattern()
{

}

void ArduinoTest::refreshSequencer()
{
    #if MIDI_TO_ARDUINO
    qDebug() <<Q_FUNC_INFO;

    QBitArray pattern = subchannelManager().getCurrentSubchannelPattern();
    int currentRelativeSubchannel = subchannelManager().getCurrentSubchannelSelelectionRelative();

    QByteArray midiData;
    midiData.resize(192);

    //qDebug() <<Q_FUNC_INFO <<"midi bytes to send:" <<midiData.size();

    for (int i = 0; i < midiData.size()/3; i++)
    {
        // 0 is color
        midiData[3*i+1] = i;    // led

        midiData[3*i+2] = 100;   // brightness


        if( pattern.at(i) )
        {
            switch (currentRelativeSubchannel)
            {
                case 0:
                    midiData[3*i+0] = MIDI_MSG_CONTROL_CHANGE | 0;
                    break;
                case 1:
                    midiData[3*i+0] = MIDI_MSG_CONTROL_CHANGE | 1;
                    break;
                case 2:
                    midiData[3*i+0] = MIDI_MSG_CONTROL_CHANGE | 2;
                    break;
                case 3:
                    midiData[3*i+0] = MIDI_MSG_CONTROL_CHANGE | 3;
                    break;
                default:
                    break;
            }
        }
        else
            midiData[3*i+0] = 0;
    }

    m_midiOut->sendData(midiData);
    //qDebug() <<Q_FUNC_INFO <<"Midi Bytes to send:" <<midiData.size();
#endif
}

void ArduinoTest::refreshSubchannelSelection()
{

}

void ArduinoTest::resetHardware()
{

}

void ArduinoTest::setStepsequencerLed(int stepLedId)
{
#if MIDI_TO_ARDUINO
    qDebug() <<Q_FUNC_INFO <<"send midi";
    quint8 data[3];
    data[0] = MIDI_MSG_CONTROL_CHANGE | 3;  // 0 = red
    data[1] = stepLedId;    // led
    data[2] = 20;  // brightness
    m_midiOut->sendDataCopy(data, 3);

    data[1] = m_lastStepSequencerLed;    // led
    data[2] = 0;  // brightness
    m_midiOut->sendDataCopy(data, 3);


    m_lastStepSequencerLed = stepLedId;
#endif
}

void ArduinoTest::setMainVolume(quint8 volume)
{

}

void ArduinoTest::slot_midiMsgReceived(quint8 *data)
{
        quint8 midiBuffer[3] = {};

        midiBuffer[0] = data[0];    // midi ch, status
        midiBuffer[1] = data[1];    // note/id
        midiBuffer[2] = data[2];    // value

        qDebug() <<Q_FUNC_INFO <<data[0] <<data[1] <<data[2];

        if( data[0] == Mosaik::MidiCommand::noteOn )
        {
            qDebug() <<Q_FUNC_INFO <<"LABBA";
            emit signal_stepButtonPressed(data[1]);
        }
        emit signal_seqMsg(data[1], data[2]);
}
