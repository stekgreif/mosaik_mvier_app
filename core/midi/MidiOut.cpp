#include "MidiOut.h"
#include <QDebug>
#include <alsa/asoundlib.h>

#define MESSAGE_NOTE_ON		0x90
#define MESSAGE_NOTE_OFF 	0x80
#define MESSAGE_CC			0xB0

/// @todo Midi Out needs HW detection functions


MidiOut::MidiOut()
{
    MidiOut("virtual");
}


MidiOut::MidiOut(QString hwId)
{
    m_midiOutDevice = hwId;
    snd_rawmidi_open(NULL, &m_midiOutHandle, m_midiOutDevice.toUtf8().constData(), SND_RAWMIDI_SYNC);

    snd_rawmidi_params_malloc(&m_params);
    snd_rawmidi_params_current(m_midiOutHandle, m_params);
    snd_rawmidi_params_set_buffer_size(m_midiOutHandle, m_params, 512);

    qDebug() <<Q_FUNC_INFO << "Midi Out Thread init with:" << m_midiOutDevice;
}


MidiOut::~MidiOut()
{
    //snd_rawmidi_params_free(params);
    snd_rawmidi_close(m_midiOutHandle);
    terminateRequest = false;
    qDebug() <<"~" <<Q_FUNC_INFO;
}


void MidiOut::run(void)
{
    qDebug() <<Q_FUNC_INFO << "Thread started.";
    while (!terminateRequest)
    qDebug() <<Q_FUNC_INFO <<"Thread terminated.";
}


void MidiOut::sendData(const QByteArray &data)
{
    snd_rawmidi_write(m_midiOutHandle, data.data(), data.size());
    //qDebug() << Q_FUNC_INFO << data.size();
}


void MidiOut::sendData(quint8 midi, quint8 id, quint8 value)
{
    quint8 data[3];
    data[0] = midi;
    data[1] = id;
    data[2] = value;

    snd_rawmidi_write(m_midiOutHandle, data, 3);

    //qDebug() << Q_FUNC_INFO <<data[0] <<data[1] <<data[2];
}


void MidiOut::sendDataCopy(quint8 data[], quint8 elements)
{
    snd_rawmidi_write(m_midiOutHandle, data, elements);
    //qDebug() << Q_FUNC_INFO <<data[0] <<data[1] <<data[2];
}

