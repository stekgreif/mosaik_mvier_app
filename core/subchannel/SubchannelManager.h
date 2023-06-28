#ifndef SUBCHANNELMANAGER_H
#define SUBCHANNELMANAGER_H

#include <core/subchannel/Subchannel.h>
#include <QList>
#include <QBitArray>
#include <QSharedPointer>
#include "core/subchannel/Sample.h"
#include "core/subchannel/Prelisten.h"


class SubchannelManager
{
    public:
        static SubchannelManager &Instance()
        {
            static SubchannelManager instance;
            return instance;
        }

        void      init(void);
        void      setSamplePath(QString path);
        quint8    getStepValue(int id);
        QBitArray hasSample(void);
        bool      hasCurrentSubchannelSample(void);

        /** steps/pattern/sequencer **/
        QBitArray  getAllHasSteps(void);
        QBitArray  getCurrentSubchannelPattern(void);
        int        getCurrentSubchannelStep(void);
        QByteArray getCurrentChannelPattern(void);
        int        getCurrentChannelStep(void);
        int        getChannelStep(int stepId);
        void       clearCurrentSubchannelPattern(void);
        void       clearCurrentChannelPattern(void);
        void       clearAllPattern(void);
        void       toggleStep(quint8 id);

        void setStepSequencerCounter(int absoluteStep);

        /** sample **/
        void loadSampleToCurrentSubchannel(QString pathAndName);
        void unloadSampleOfCurrentSubchannel(void);
        void unloadSamplesOfCurrentChannel(void);
        void unloadAllSamples(void);
        QBitArray getPlayingSubchannels(void);

        /** prelisten **/
        void loadSampleToPrelisten(QString pathAndName);
        void currentSubchannelToPrelisten(bool status);
        void prelistenCurrentSubchannelSample(void);

        /** mute **/
        QBitArray getMuteStates(void);
        void toggleMuteState(int id);
        void unmuteAll(void);
        void lastMuteStates(void);

        QString getCurrentSamplePathAndName(void);
        QSharedPointer<Sample> getSharedPointerToSample(void);
        QSharedPointer<Sample> getSharedPointerToSample(int subchannel);
        TwoChannelFrame_t  getFrame(int subchannelId);
        FourChannelFrame_t getFourChannelAudioTestFrame(void);
        FourChannelFrame_t getFourChannelAudioFrame(void);
        void  setCurrentSubchannelVolumeRelative(float vol);
        float getCurrentSubchannelVolume(void);
        void  setCurrentPan(float relVal);
        float getCurrentPan() const;
        float getSubchannelVolume(int subchId);
        void  togglePlayDirection(void);
        void  setCurrentPlayDirection(bool direction);
        bool  getCurrentPlayDirection(void);

        /** sub/channel selection **/
        void  setCurrentSubchannelSelection(int id);
        void  setCurrentSubchannelSelectionRelative(int id);
        int   getCurrentSubchannelSelectionRelative(void);
        int   getCurrentSubchannelSelection(void);
        void  selectNextSubChannel();
        void  setCurrentChannelSelection(int id);
        void  setCurrentChannelRelative(int change);
        int   getCurrentChannelSelection(void);
        void  getAudioParameter();
        QList<int> getSubchannelIdsOfCurrentChannel(void);

        /** envelope **/
        void setCurrentStartPointRel(  float start  );
        void setCurrentEndPointRel(    float end    );
        void setCurrentFadeInPointRel( float fadeIn );
        void setCurrentFadeOutPointRel(float fadeOut);
        envelope_t getCurrentEnvelope(void);
        envelope_t getEnvelope(int id);

        /** global **/
        float getBpm() const;
        void  setBpm(float bpm);
        void  setBpmRelative(float change);
        float getMainVolume() const;
        void  setMainVolumeRelative(float relVal);
        float getPreVolume() const;
        void  setPreVolumeRelative(float relVal);
        void  setPreVolumeAbs(float absVal);

    protected:
        SubchannelManager();
        SubchannelManager(const SubchannelManager& other) {Q_UNUSED(other);}
        ~SubchannelManager();


    private:
        QBitArray m_lastMuteStates;
        bool checkIfAllSubchannelsAreUnmuted(void);
        int m_currentSubchannel;
        int m_currentChannel;
        int m_currentStep;
        float m_bpm;
        float m_mainVolume;
        float m_preVolume;

        QList <Subchannel*> m_subchannel;

        float m_currentSubchannelToHeadphones;
        Prelisten *m_prelisten;
        Prelisten *m_prelistenCurrentSubchannelSample;
};

inline SubchannelManager& subchannelManager() {return SubchannelManager::Instance(); }

#endif // SUBCHANNELMANAGER_H
