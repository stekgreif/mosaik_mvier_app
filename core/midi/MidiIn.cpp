#include "MidiIn.h"
#include <QDebug>
#include <QtCore>


MidiIn::MidiIn()
{
    snd_rawmidi_open(&midi_in_handle, NULL, "virtual", SND_RAWMIDI_SYNC);
    qDebug() <<Q_FUNC_INFO << "Midi In Thread init with virtual.";
}


MidiIn::MidiIn(QString hwId)
{
    snd_rawmidi_open(&midi_in_handle, NULL, hwId.toUtf8().constData(), SND_RAWMIDI_SYNC);
    qDebug() <<Q_FUNC_INFO << "Init with:" << hwId ;
}


MidiIn::~MidiIn()
{
    snd_rawmidi_close(midi_in_handle);
    terminateRequest = false;
    qDebug() <<"~" <<Q_FUNC_INFO;
}


void MidiIn::run(void)
{
    qDebug() <<Q_FUNC_INFO <<"Thread started.";

    while (!terminateRequest)
    {
        snd_rawmidi_read(midi_in_handle, midi_in_buffer, 3);
        emit signal_midiMsgReceived(midi_in_buffer);
        qDebug() <<Q_FUNC_INFO <<"Midi received";
    }

    qDebug() <<Q_FUNC_INFO <<"Thread terminated.";
}


