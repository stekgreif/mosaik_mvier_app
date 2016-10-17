#ifndef HWUI_01_H
#define HWUI_01_H

#include <QObject>
#include "core/midi/MidiIn.h"
#include "core/midi/MidiOut.h"

class Hwui_01 : public QObject
{
	Q_OBJECT

	public:
		Hwui_01(QString hwPort);
		~Hwui_01();

	public slots:
		void slot_midiMsgReceived(quint8* data);

	protected:
		MidiOut *m_midiOut;
		MidiIn  *m_midiIn;

	private:
		QString m_hwPort;
		bool m_mute;

	signals:
		void signal_button00Pressed();
		void signal_button01Pressed();
		void signal_button02Pressed();
		void signal_button03Pressed();
		void signal_button04Pressed(bool state);
		void signal_button05Pressed();
		void signal_button06Pressed();
		void signal_button07Pressed();
		void signal_button08Pressed();
		void signal_button09Pressed();
		void signal_button10Pressed();
		void signal_button11Pressed();
};

#endif // HWUI_01_H
