#ifndef ALSA_PCM_H
#define ALSA_PCM_H

/*  aplay -l
    hw:X,Y
    X: card number
    Y: device number */

#include "core/ppthread.h"
#include "alsa/asoundlib.h"
#include <atomic>
#include "Settings.h"

#define CHANNELS   SETTINGS_AUDIO_CHANNEL

#define NUM_OF_PERIODS  2
#define FRAMES          512 // 128, 2048
#define BUFFER_SIZE     FRAMES * CHANNELS


class AlsaPcm : public PPThread
{
    Q_OBJECT

    public:
        AlsaPcm();
        ~AlsaPcm();

        quint32 getStepCounter() const;
        int getNumberOfFrames() const;

    public slots:
        void slot_bpmChanged(float bpm);

    private:
        virtual void run();             //thread function

        int err;
        snd_pcm_t *m_pcmHandle;
        snd_pcm_hw_params_t *m_hwParams;
        snd_pcm_sw_params_t *m_swParams;
        snd_pcm_sframes_t m_framesToDeliver;

        int playbackCallback(snd_pcm_sframes_t nFrames);

        float   m_pcmBuffer[FRAMES*2];
        float   m_squareBuffer[FRAMES*2];

        quint32 m_sampleRate;
        quint64 m_frameCnt;
        qint32  m_barCnt;
        qint64  m_framesPerStep;
        bool    m_newBpmValue;
        float   m_pendingBpmValue;

        std::atomic<quint32> m_stepCnt;
};

#endif // ALSA_PCM_H
