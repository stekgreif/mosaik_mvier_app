#ifndef VIRTUALMIDIDEVICE_H
#define VIRTUALMIDIDEVICE_H

#include "core/midi/devices/MidiDevice.h"
#include "core/midi/MidiOut.h"
#include "core/midi/MidiIn.h"

#include <QObject>
#include <QWidget>

class VirtualMidiDevice : public MidiDevice
{
    Q_OBJECT

    public:
        VirtualMidiDevice();
        ~VirtualMidiDevice();

        void setStepLed(int i) Q_DECL_OVERRIDE;
        void sendRawData(const QByteArray &data) Q_DECL_OVERRIDE;
        void setSubchannelPattern(void) Q_DECL_OVERRIDE;
        void setChannelPattern(void) Q_DECL_OVERRIDE;
        void refreshSequencer(void) Q_DECL_OVERRIDE;
        void resetHardware(void) Q_DECL_OVERRIDE;
        void refreshSubchannelSelection(void) Q_DECL_OVERRIDE;
        void setStepsequencerLed(int stepLedId) Q_DECL_OVERRIDE;
        void setMainVolume(quint8 volume) Q_DECL_OVERRIDE;
};

#endif // VIRTUALMIDIDEVICE_H
