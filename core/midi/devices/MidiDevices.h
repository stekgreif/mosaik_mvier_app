#ifndef MIDIDEVICES_H
#define MIDIDEVICES_H

#include "core/midi/MidiOut.h"
#include "core/midi/MidiIn.h"
#include <QObject>


class MidiDevices : public QObject
{
	Q_OBJECT

	public:
		MidiDevices();
		~MidiDevices();
		void setDeviceId(int id);
		void setDevideType(int type);
		int  getDeviceId(void);
		int  getDeviceType(void);
		int  readDeviceHwPort();

	protected:
		MidiOut *m_midiOut;
		MidiIn  *m_midiIn;

	private:
		int m_deviceId = 0;
		int m_deviceType = 0;
};

#endif // MIDIDEVICES_H
