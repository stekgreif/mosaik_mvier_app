#include "VirtualMidiDevice.h"

#include <QDebug>

VirtualMidiDevice::VirtualMidiDevice()
{
    qDebug() <<Q_FUNC_INFO <<"Connect to virtual midi.";
    m_midiOut = new MidiOut();
    m_midiIn  = new MidiIn ();
    m_midiIn->start();
}

VirtualMidiDevice::~VirtualMidiDevice()
{
    delete m_midiOut;
    delete m_midiIn;
    qDebug() <<"~" <<Q_FUNC_INFO;
}

void VirtualMidiDevice::setStepLed(int i)
{
    Q_UNUSED(i);
    qDebug() <<Q_FUNC_INFO;
}

void VirtualMidiDevice::sendRawData(const QByteArray &data)
{
    Q_UNUSED(data);
    qDebug() <<Q_FUNC_INFO;
}

void VirtualMidiDevice::setSubchannelPattern()
{
    qDebug() <<Q_FUNC_INFO;
}

void VirtualMidiDevice::setChannelPattern()
{
    qDebug() <<Q_FUNC_INFO;
}

void VirtualMidiDevice::refreshSequencer()
{
    qDebug() <<Q_FUNC_INFO;
}

void VirtualMidiDevice::resetHardware()
{
    qDebug() <<Q_FUNC_INFO;
}

void VirtualMidiDevice::refreshSubchannelSelection()
{
    qDebug() <<Q_FUNC_INFO;
}

void VirtualMidiDevice::setStepsequencerLed(int stepLedId)
{
    Q_UNUSED(stepLedId);
    //qDebug() <<Q_FUNC_INFO;
}

void VirtualMidiDevice::setMainVolume(quint8 volume)
{
    Q_UNUSED(volume);
}

