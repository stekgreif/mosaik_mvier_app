#include "ArduinoTest.h"

#include "MosaikMini.h"
#include "core/midi/MidiNames.h"
#include "Settings.h"
#include "MosaikTypes.h"
#include "core/midi/MidiNames.h"
#include "core/subchannel/SubchannelManager.h"

#include <QObject>
#include "QDebug"




ArduinoTest::ArduinoTest()
{
    m_lastStepSequencerLed = 0;
    qDebug() <<Q_FUNC_INFO <<"Get connected midi device" <<settings().getConnectedMidiDevicePort();
    m_midiOut = new MidiOut(settings().getConnectedMidiDevicePort());
}

ArduinoTest::~ArduinoTest()
{

}



void ArduinoTest::setStepLed(int i)
{
    qDebug() <<Q_FUNC_INFO <<"send midi";
    quint8 data[3];
    data[0] = MIDI_MSG_CONTROL_CHANGE | 2;  // 0 = red
    data[1] = i;    // led
    data[2] = 55;  // brightness
    m_midiOut->sendDataCopy(data, 3);
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
    qDebug() <<Q_FUNC_INFO;

    QBitArray pattern = subchannelManager().getCurrentSubchannelPattern();
    int currentRelativeSubchannel = subchannelManager().getCurrentSubchannelSelelectionRelative();

    QByteArray midiData;
    midiData.resize(192);

    //qDebug() <<Q_FUNC_INFO <<"midi bytes to send:" <<midiData.size();

    for (int i = 0; i < midiData.size()/3; i++)
    {

        midiData[3*i+2] = 25;
        midiData[3*i+1] = i;

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
            midiData[3*i+2] = 0;
    }

    m_midiOut->sendData(midiData);
    //qDebug() <<Q_FUNC_INFO <<"Midi Bytes to send:" <<midiData.size();
}

void ArduinoTest::refreshSubchannelSelection()
{

}

void ArduinoTest::resetHardware()
{

}

void ArduinoTest::setStepsequencerLed(int stepLedId)
{
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
}

void ArduinoTest::setMainVolume(quint8 volume)
{

}
