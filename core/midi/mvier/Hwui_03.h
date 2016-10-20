#ifndef HWUI_03_H
#define HWUI_03_H

#include <QObject>
#include "core/midi/MidiIn.h"
#include "core/midi/MidiOut.h"

class Hwui_03 : public QObject
{
	Q_OBJECT

	public:
		Hwui_03(QString hwPort);
		~Hwui_03();

	public slots:
		void slot_midiMsgReceived(quint8* data);

	protected:
		MidiOut *m_midiOut;
		MidiIn  *m_midiIn;

	private:
		QString m_hwPort;
		bool m_curSubToPre;

	signals:
		void signal_button00Pressed();
		void signal_button01Pressed();
		void signal_button02Pressed();
		void signal_button03Pressed();
		void signal_button04Pressed();
		void signal_button05Pressed(int up);
		void signal_button06Pressed(bool state);
		void signal_button07Pressed();
		void signal_button08Pressed(int down);
		void signal_button09Pressed();
		void signal_button10Pressed(int down);
		void signal_button11Pressed(int up);
};

#endif // HWUI_03_H
