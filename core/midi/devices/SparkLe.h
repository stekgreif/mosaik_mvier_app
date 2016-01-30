#ifndef SPARKLE_H
#define SPARKLE_H

#include "core/midi/devices/MidiDevice.h"
#include "core/midi/MidiOut.h"
#include "core/midi/MidiIn.h"

#include <QObject>
#include <QWidget>

class SparkLe : public MidiDevice
{
    Q_OBJECT

    public:
        SparkLe();
        ~SparkLe();

        void setStepLed(int i) Q_DECL_OVERRIDE;
        void sendRawData(const QByteArray &data) Q_DECL_OVERRIDE;
        void setSubchannelPattern(void) Q_DECL_OVERRIDE;
        void setChannelPattern(void) Q_DECL_OVERRIDE;
        void refreshSequencer(void) Q_DECL_OVERRIDE;
        void refreshSubchannelSelection(void) Q_DECL_OVERRIDE;
        void resetHardware(void) Q_DECL_OVERRIDE;
        void setStepsequencerLed(int stepLedId) Q_DECL_OVERRIDE;
        void setMainVolume(quint8 volume) Q_DECL_OVERRIDE;

    signals:
        void signal_bpmChanged(float val);
        void signal_mainVolume(quint8 relVal);
        void signal_currentSubchannelVol(float relVal);
        void signal_currentSubchannelPanRel(float relVal);

    public slots:
        void slot_midiMsgReceived(quint8* data);


    private:
        void midiNoteOnReceived(quint8 id, quint8 value);
        void midiNoteOffReceived(quint8 id, quint8 value);
        void midiCcReceived(quint8 id, quint8 value);
        void changePatternSectionFocus(int section);
        quint8 m_patternSection;
        int m_lastStepSequencerLed;
};

#endif // SPARKLE_H
