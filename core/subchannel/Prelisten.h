#ifndef PRELISTEN_H
#define PRELISTEN_H

#include <QSharedPointer>

#include "core/subchannel/Sample.h"


class Prelisten
{
    public:
        Prelisten();
        ~Prelisten();

        TwoChannelFrame_t getFrame(void);

        void playSample(QString pathAndName);

    private:
        Sample *m_sample;
        bool m_playNewSample;
        bool m_finishedPlaying;
        int  m_frameCounter;
        int  m_frameNumbers;
        float *m_sampleBuffer;
        int m_channels;
};

#endif // PRELISTEN_H
