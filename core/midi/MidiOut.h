#ifndef MIDIOUT_H
#define MIDIOUT_H

#include <alsa/asoundlib.h>
#include <QString>
#include "core/ppthread.h"


class MidiOut : public PPThread
{
    Q_OBJECT

    public:
        MidiOut();
        MidiOut(QString hwId);
        ~MidiOut();

        void sendData(const QByteArray &data);
        void sendData(quint8 midi, quint8 id, quint8 value);
        void sendDataCopy(quint8 data[], quint8 elements);
        void reset();

    private:
        virtual void run();

        QString        m_midiOutDevice;
        snd_rawmidi_t* m_midiOutHandle;


        snd_rawmidi_params_t *m_params;
};
#endif // MIDIOUT_H





