#ifndef HWUI_02_H
#define HWUI_02_H

#include <QObject>
#include "core/midi/MidiIn.h"
#include "core/midi/MidiOut.h"


class Hwui_02 : public QObject
{
	Q_OBJECT

	public:
		Hwui_02(QString hwPort);
		~Hwui_02();

	public slots:
		void slot_midiMsgReceived(quint8* data);

	signals:
		void signal_erpChanged(quint8 v1, qint8 v2);

	protected:
		MidiOut *m_midiOut;
		MidiIn  *m_midiIn;

	private:
		QString m_hwPort;
};

#endif // HWUI_02_H
