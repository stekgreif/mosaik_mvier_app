#ifndef ARDUINOTEST_H
#define ARDUINOTEST_H

#include "core/midi/devices/MidiDevice.h"
#include "core/midi/MidiOut.h"
#include "core/midi/MidiIn.h"

#include <QObject>
#include <QBitArray>

class ArduinoTest : public MidiDevice
{
    Q_OBJECT

    public:
        ArduinoTest();
        ~ArduinoTest();
        void setStepLed(int i) Q_DECL_OVERRIDE;
        void sendRawData(const QByteArray &data) Q_DECL_OVERRIDE;
        void setSubchannelPattern(void) Q_DECL_OVERRIDE;
        void setChannelPattern(void) Q_DECL_OVERRIDE;
        void refreshSequencer(void) Q_DECL_OVERRIDE;
        void refreshSubchannelSelection(void) Q_DECL_OVERRIDE;
        void resetHardware(void) Q_DECL_OVERRIDE;
        void setStepsequencerLed(int stepLedId) Q_DECL_OVERRIDE;
        void setMainVolume(quint8 volume) Q_DECL_OVERRIDE;


    public slots:
        void slot_midiMsgReceived(quint8* data);

    private:
        int m_lastStepSequencerLed;

		int m_lastRelSubCh;
		QBitArray m_lastSubPat;

    signals:
        void signal_seqMsg(quint8 id, quint8 val);
};

#endif // ARDUINOTEST_H
