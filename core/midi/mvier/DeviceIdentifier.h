#ifndef DEVICEIDENTIFIER_H
#define DEVICEIDENTIFIER_H

#include "core/midi/MidiIn.h"
#include "core/midi/MidiOut.h"
#include <QObject>

class DeviceIdentifier : public QObject
{
	Q_OBJECT

	public:
		DeviceIdentifier();
		//DeviceIdentifier(QString hwPort);
		~DeviceIdentifier();


		//QList<QString> getDeviceInfo(void);
		void midiConnect(QString hwPort);
		void midiScanConnection(void);
		void midiDisconnect(void);
		int  getDeviceId(void);

	public slots:
		void slot_midiMsgReceived(quint8* data);

	protected:
		MidiOut *m_midiOut;
		MidiIn  *m_midiIn;
		QString m_hwPort;
		uint8_t m_id;

	private:
		bool m_idAvailable;
};

#endif // DEVICEIDENTIFIER_H
