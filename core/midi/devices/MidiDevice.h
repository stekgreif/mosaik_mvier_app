#ifndef MIDIDEVICE_H
#define MIDIDEVICE_H

#include "core/midi/MidiOut.h"
#include "core/midi/MidiIn.h"
#include <QObject>



class MidiDevice : public QObject
{
    Q_OBJECT

    public:
        MidiDevice();
        ~MidiDevice();

        virtual void setStepLed(int i) = 0;
        virtual void sendRawData(const QByteArray &data) = 0;
        virtual void setSubchannelPattern(void) = 0;
        virtual void setChannelPattern(void) = 0;
        virtual void refreshSequencer(void) = 0;
        virtual void resetHardware(void) = 0;
        virtual void refreshSubchannelSelection(void) = 0;
        virtual void setStepsequencerLed(int stepLedId) = 0;
        virtual void setMainVolume(quint8 volume) = 0;
        virtual void subToPreLed(bool state);

    signals:
        void signal_rawMidiMsg(quint8 status, quint8 id, quint8 val);
        void signal_erpChanged(quint8 id, qint8 val);
        void signal_encChanged(int val);
        void signal_menuEncoderPushed(void);
        void signal_menuNavigationUpPushed(void);
        void signal_menuNavigationDownPushed(void);
        void signal_menuNavigation(int direction);
        void signal_ahcMsg(quint8 val);
        void signal_bpmChanged(float val);
        void signal_mainVolume(float relVal);
        void signal_currentSubchannelVol(float relVal);
        void signal_currentSubchannelPanRel(float relVal);
        void signal_loadSample(void);
        void signal_prelistenBrowserSample(void);
        void signal_headphoneVolume(float absVal);
        void signal_currentPan(float relVal);
        void signal_currentSubchannelToPre(bool state);
        void signal_unmuteAll(void);
        void signal_lastMute(void);

        void signal_browserOpenFolder(void);
        void signal_browserCloseFolder(void);

        /** new signal functions **/
        void signal_stepButtonPressed(int id);
        void signal_selectPageSubchannel(void);
        void signal_selectPageBrowser(void);
        void signal_selectChannel(int id);
        void signal_setPathId(int id);
        void signal_muteAndSolo(bool state);
        void signal_prelistenSubchannelSample(bool);

        void signal_functionRightButton00Pressed(void);
        void signal_functionRightButton01Pressed(void);
        void signal_functionRightButton02Pressed(void);
        void signal_functionRightButton03Pressed(void);
        void signal_functionRightButton04Pressed(void);
        void signal_functionRightButton05Pressed(void);
        void signal_functionRightButton06Pressed(void);
        void signal_functionRightButton07Pressed(void);

        void signal_functionRightButton08Pressed(void);
        void signal_functionRightButton09Pressed(void);
        void signal_functionRightButton10Pressed(void);
        void signal_functionRightButton11Pressed(void);
        void signal_functionRightButton12Pressed(void);
        void signal_functionRightButton13Pressed(void);
        void signal_functionRightButton14Pressed(void);
        void signal_functionRightButton15Pressed(void);

        void signal_functionLeftButton00Pressed(void);
        void signal_functionLeftButton01Pressed(void);
        void signal_functionLeftButton02Pressed(void);
        void signal_functionLeftButton03Pressed(void);
        void signal_functionLeftButton04Pressed(void);
        void signal_functionLeftButton05Pressed(void);
        void signal_functionLeftButton06Pressed(void);
        void signal_functionLeftButton07Pressed(void);

        void signal_functionLeftButton08Pressed(void);
        void signal_functionLeftButton09Pressed(void);
        void signal_functionLeftButton10Pressed(void);
        void signal_functionLeftButton11Pressed(void);
        void signal_functionLeftButton12Pressed(void);
        void signal_functionLeftButton13Pressed(void);
        void signal_functionLeftButton14Pressed(void);
        void signal_functionLeftButton15Pressed(void);

        void signal_functionSelectSubchannelRelative(int id);

    protected:
        MidiOut *m_midiOut;
        MidiIn  *m_midiIn;
};

#endif // MIDIDEVICE_H
