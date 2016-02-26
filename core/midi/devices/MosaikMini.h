#ifndef MOSAIKMINI_H
#define MOSAIKMINI_H

#include "core/midi/devices/MidiDevice.h"
#include "core/midi/MidiOut.h"
#include "core/midi/MidiIn.h"

#include <QObject>
#include <QWidget>

class MosaikMini : public MidiDevice
{
    Q_OBJECT

    public:
        MosaikMini();
        ~MosaikMini();

        void setStepLed(int i) Q_DECL_OVERRIDE;
        void sendRawData(const QByteArray &data) Q_DECL_OVERRIDE;
        void setSubchannelPattern(void) Q_DECL_OVERRIDE;
        void setChannelPattern(void) Q_DECL_OVERRIDE;  
        void refreshSequencer(void) Q_DECL_OVERRIDE;
        void resetHardware(void) Q_DECL_OVERRIDE;
        void refreshSubchannelSelection(void) Q_DECL_OVERRIDE;
        void setStepsequencerLed(int stepLedId) Q_DECL_OVERRIDE;
        void setMainVolume(quint8 volume);
        void subToPreLed(bool state) Q_DECL_OVERRIDE;

    public slots:
        void slot_midiMsgReceived(quint8* data);

    signals:
        void signal_encChanged(int val);
        void signal_selectPageSubchannel(void);
        void signal_selectPageBrowser(void);
        void signal_selectChannel(int id);
        void signal_setPathId(int id);

        void signal_menuEncoderPushed(void);
        void signal_menuNavigationUpPushed(void);
        void signal_menuNavigationDownPushed(void);
        void signal_menuNavigation(int direction);

        void signal_browserOpenFolder(void);
        void signal_browserCloseFolder(void);

        void signal_bpmChanged(float val);
        void signal_mainVolume(float relVal);
        void signal_loadSample(void);
        void signal_headphoneVolume(float absVal);
        void signal_currentPan(float relVal);

        void signal_currentSubchannelToPre(bool state);
        void signal_muteAndSolo(bool state);
        void signal_unmuteAll(void);
        void signal_lastMute(void);

        void signal_prelistenBrowserSample(void);
        void signal_prelistenSubchannelSample(void);

#if 0
        /** new signal functions -> these prototypes are already in MidiDevice**/
        void signal_functionRightButton00Pressed(void);
        void signal_functionRightButton01Pressed(void);
        void signal_functionRightButton02Pressed(void);
        void signal_functionRightButton03Pressed(void);
        void signal_functionRightButton04Pressed(void);
        void signal_functionRightButton05Pressed(void);
        void signal_functionRightButton06Pressed(void);
        void signal_functionRightButton07Pressed(void);
#endif

        /** old signal functions -> make obsolete **/
        void signal_seqMsg(quint8 id, quint8 val);
        void signal_fnlMsg(quint8 id, quint8 val);
        void signal_fnrMsg(quint8 id, quint8 val);
        void signal_menMsg(quint8 id, quint8 val);

        void signal_erpMsg(quint8 id, quint8 val);
        void signal_padMsg(quint8 id, quint8 val);
        void signal_crfMsg(quint8 id, quint8 val);
        void signal_ahoMsg(quint8 id, quint8 val);

        void signal_ahcMsg(quint8 id, quint8 val);
        void signal_ahcMsg(quint8 val);
        void signal_apaMsg(quint8 id, quint8 val);
        void signal_ainMsg(quint8 id, quint8 val);
        void signal_sprMsg(quint8 id, quint8 val);

        void signal_sysMsg(quint8 id, quint8 val);

        void signal_sparkMsg(quint8 id, int sigVal);

    private:
        bool m_singlePatternView;
        void toggleSinglePatternView(void);
        int m_lastStepSequencerLed;
        int getColorValue(int id);

        bool m_shiftBpm;
        bool m_shiftMainVol;

        bool m_shiftPan;
        bool m_shiftPitch;
        bool m_shiftPlayDir;

        bool m_tglSubToPre;
        bool m_mute;


};

#endif // MOSAIKMINI_H



#if 0
#ifndef MIDIINPARSER_H
#define MIDIINPARSER_H

#include "MidiIn.h"
#include <QObject>
#include <QWidget>

class MidiInParser : public QWidget
{
    Q_OBJECT
public:
    MidiInParser();
    ~MidiInParser();

private:
    MidiIn *m_midiIn;

public slots:
    void slot_midiMsgReceived(quint8* data);

signals:
    void signal_seqMsg(quint8 id, quint8 val);
    void signal_fnlMsg(quint8 id, quint8 val);
    void signal_fnrMsg(quint8 id, quint8 val);
    void signal_menMsg(quint8 id, quint8 val);

    void signal_erpMsg(quint8 id, quint8 val);
    void signal_padMsg(quint8 id, quint8 val);
    void signal_crfMsg(quint8 id, quint8 val);
    void signal_ahoMsg(quint8 id, quint8 val);

    void signal_ahcMsg(quint8 id, quint8 val);
    void signal_apaMsg(quint8 id, quint8 val);
    void signal_ainMsg(quint8 id, quint8 val);
    void signal_sprMsg(quint8 id, quint8 val);

    void signal_sysMsg(quint8 id, quint8 val);

    void signal_sparkMsg(quint8 id, int sigVal);
};

#endif // MIDIINPARSER_H

#endif




#if 0

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


#endif
