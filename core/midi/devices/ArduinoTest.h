#ifndef ARDUINOTEST_H
#define ARDUINOTEST_H

#include "core/midi/devices/MidiDevice.h"
#include "core/midi/MidiOut.h"
#include "core/midi/MidiIn.h"

#include <QObject>

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

    private:
        int m_lastStepSequencerLed;
};

#endif // ARDUINOTEST_H
