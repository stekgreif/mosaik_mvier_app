#ifndef MIDIIN_H
#define MIDIIN_H

#include <alsa/asoundlib.h>
#include "core/ppthread.h"


class MidiIn : public PPThread
{
    Q_OBJECT

    public:
        MidiIn();
        MidiIn(QString hwId);
        ~MidiIn();

    signals:
        void signal_midiMsgReceived(quint8* midi_byte);

    private:
        virtual void run(void); //thread function

        snd_rawmidi_t* midi_in_handle;
        quint8 midi_in_buffer[3];
};

#endif // MIDI_IN_H
