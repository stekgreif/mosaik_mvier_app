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
		bool m_shiftBpm;
		bool m_shiftMainVol;
		bool m_shiftPan;
		bool m_shiftPitch;
		bool m_shiftPlayDir;
		bool m_tglSubToPre;
		bool m_shiftSubVol;

	signals:
		//void signal_encChanged(int val);
		void signal_mainVolume(float relVal);
		void signal_currentPan(float relVal);
		void signal_bpmChanged(float val);
		void signal_erpChanged(quint8 v1, qint8 v2);

		void signal_button00Pressed();
		void signal_button01Pressed();
		void signal_button02Pressed();
		void signal_button03Pressed();
		void signal_button04Pressed();
		void signal_button05Pressed();
		void signal_button06Pressed();
		void signal_button07Pressed(bool state);	// mute
		void signal_button08Pressed();				// unmute all
		void signal_button09Pressed();				// last mute
		void signal_button10Pressed();
		void signal_button11Pressed();
};

#endif // HWUI_01_H
