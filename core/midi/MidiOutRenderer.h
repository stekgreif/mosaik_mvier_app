#ifndef MIDIOUTRENDERER_H
#define MIDIOUTRENDERER_H

#include "core/midi/MidiOut.h"



class MidiOutRenderer
{
    public:
        MidiOutRenderer();
        ~MidiOutRenderer();

        void setFnlLed(quint8 id, quint8 color);
        void rstFnlLeds(void);

        void setSeqLed(quint8 id, quint8 color);
        void rstSeqLeds(void);

        void setMenLed(quint8 id, quint8 color);
        void rstMenLeds(void);

        void setCrfLed(quint8 id, quint8 color);
        void rstCrfLeds(void);

        void setFnrLed(quint8 id, quint8 color);
        void rstFnrLeds(void);

        void setApaVolume(quint8 val);
        void rstApa(void);

        void rstSys(void);

    private:
        MidiOut *m_midiOut;
};

#endif // MIDIOUTRENDERER_H
