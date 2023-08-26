#include "MidiOut.h"
#include <QDebug>
#include <alsa/asoundlib.h>

#define MESSAGE_NOTE_ON		0x90
#define MESSAGE_NOTE_OFF 	0x80
#define MESSAGE_CC			0xB0

/// @todo Midi Out needs HW detection functions
#define MIDI_OUT 1

MidiOut::MidiOut()
{
#if MIDI_OUT
    MidiOut("virtual");
#endif
}


MidiOut::MidiOut(QString hwId)
{
#if MIDI_OUT
    m_midiOutDevice = hwId;
    snd_rawmidi_open(NULL, &m_midiOutHandle, m_midiOutDevice.toUtf8().constData(), SND_RAWMIDI_SYNC);

    snd_rawmidi_params_malloc(&m_params);
    snd_rawmidi_params_current(m_midiOutHandle, m_params);
    snd_rawmidi_params_set_buffer_size(m_midiOutHandle, m_params, 512);

    qDebug() <<Q_FUNC_INFO << "Midi Out Thread init with:" << m_midiOutDevice;
#endif
}


MidiOut::~MidiOut()
{
#if MIDI_OUT
    //snd_rawmidi_params_free(params);
    snd_rawmidi_close(m_midiOutHandle);
    terminateRequest = false;
    qDebug() <<"~" <<Q_FUNC_INFO;
#endif
}


void MidiOut::run(void)
{
#if MIDI_OUT
    qDebug() <<Q_FUNC_INFO << "Thread started.";
    while (!terminateRequest)
    qDebug() <<Q_FUNC_INFO <<"Thread terminated.";
#endif
}


void MidiOut::sendData(const QByteArray &data)
{
#if MIDI_OUT
    snd_rawmidi_write(m_midiOutHandle, data.data(), data.size());
    qDebug() << Q_FUNC_INFO << data.size();
#endif
}


void MidiOut::sendData(quint8 midi, quint8 id, quint8 value)
{
#if MIDI_OUT
    quint8 data[3];
    data[0] = midi;
    data[1] = id;
    data[2] = value;

    snd_rawmidi_write(m_midiOutHandle, data, 3);
#endif
    qDebug() << Q_FUNC_INFO <<data[0] <<data[1] <<data[2];
}


void MidiOut::sendDataCopy(quint8 data[], quint8 elements)
{
#if MIDI_OUT
    snd_rawmidi_write(m_midiOutHandle, data, elements);
    qDebug() << Q_FUNC_INFO <<data[0] <<data[1] <<data[2];
#endif
}

