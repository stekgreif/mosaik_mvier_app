#ifndef ENVELOPE_H
#define ENVELOPE_H

#include "MosaikTypes.h"


/*    m_envelope.start = 0;
    m_envelope.end = 0;
    m_envelope.fadeIn = 0;
    m_envelope.fadeOut = 0;
    m_envelope.offset = 0;   */


class Envelope
{
    public:
        Envelope();
        ~Envelope();

        envelope_t getEnvelope(void);

        void setStartPointRel(      float start  );
        void setEndPointRel(        float end    );
        void setFadeInPointRel(     float fadeIn );
        void setFadeOutPointRel(    float fadeOut);

        void setRelativeStart(float start);
        void setRelativeEnd(float end);
        void setRelativeFadeIn(float fadeIn);
        void setRelativeFadeOut(float fadeOut);

        void setStartOffsetPoint(float offset );

        void resetAll(void);

    private:
        envelope_t m_envelope;
};

#endif // ENVELOPE_H
