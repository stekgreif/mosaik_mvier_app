#include "ArduinoTest.h"

#include "core/midi/MidiNames.h"
#include "QDebug"
#include "Settings.h"



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
