#ifndef SUBCHANNEL_H
#define SUBCHANNEL_H

#include <QList>
#include <QString>
#include <QSharedPointer>
#include "core/subchannel/Sample.h"
#include "core/subchannel/Envelope.h"
#include "core/subchannel/Sequencer.h"
#include "core/subchannel/AudioParameter.h"


class Subchannel
{
    public:
        Subchannel(int id);
        ~Subchannel();

        /** envelope **/
        Envelope *m_envelope;
        void setStartPointRel(   float start  );
        void setEndPointRel(     float end    );
        void setFadeInPointRel(  float fadeIn );
        void setFadeOutPointRel( float fadeOut);
        void setStartOffsetPoint(float offset );
        envelope_t getEnvelope(void);

        /** audio parameter **/
        void  setVolumeRel(float vol);
        float getVolume(void);
        void  setPan(float relVal);
        float getPan(void);
        void  togglePlayDirection();
        void  setPlayDirection(bool direction);
        bool  getPlayDirection(void);
        //bool  getPlayDirectionForward();
        bool  isMute() const;
        void  setMute(bool state);

        /** Sample **/
        bool hasSample(void);
        void loadSample(QString pathAndName);
        void unloadSample(void);
        bool isPlaying(void);

        TwoChannelFrame_t getFrame(void);
        TwoChannelFrame_t getTestFrame(void);
        TwoChannelFrame_t getTestFrameWithSteps(void);
        QSharedPointer<Sample> getSharedSamplePtr(void);

        /** sequencer **/
        void      toggleStep(int stepId);
        void      clearPattern(void);
        quint8    getStepValue(int stepId);
        QString   getPatternForPrinting(void);
        QBitArray getPattern(void);
        bool      hasSteps(void);
        void      updateRelativeStepCounter(int step);  //0..63

    private:
        QSharedPointer<Sample> m_sharedSamplePtr;
        int m_subchannelId;
        QList<int> *m_pattern;
        void printSubchannelPatternsToDebug(void);

        Sample *m_sample = nullptr;
        bool m_hasSample;
        int  m_stepCounter;
        int  m_frameReturnState;

        float m_volume;
        float m_pan;  // 0:left  0.5:center  1:right
        float m_audioMute; // 0.0: muted  1.0 not muted

        int  m_tempCnt;
        bool m_tgl;

        int m_audioFramesRemain;
        int m_audioState;
        int m_audioOldStep;
        bool m_audioStepChanged;

        /** IMPLEMENT! **/
        int m_sampleId;

        /** 2015-04-24 parameter for new trigger algorithm **/
        bool m_finishedPlaying;
        int  m_step;
        int  m_alsaStep;
        bool m_stepHasChanged;
        int  m_frameCounter;
        int  m_frameNumbers;
        int  m_channels;
        bool m_playForward;
        float *m_sampleBuffer;

        int m_envEnd;
};

#endif // SUBCHANNEL_H
