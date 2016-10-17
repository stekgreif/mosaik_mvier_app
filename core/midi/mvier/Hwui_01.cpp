#include "Hwui_01.h"

#include "core/midi/MidiNames.h"
#include "Settings.h"
#include "MosaikTypes.h"

#include <QDebug>




Hwui_01::Hwui_01(QString hwPort)
{
	m_hwPort = hwPort;

	m_midiOut = new MidiOut(m_hwPort);

	m_midiIn  = new MidiIn(m_hwPort);
	connect(m_midiIn, SIGNAL(signal_midiMsgReceived(quint8*)), this, SLOT(slot_midiMsgReceived(quint8*)) );
	m_midiIn->start();

	m_mute = false;
}


Hwui_01::~Hwui_01()
{
	m_midiIn->stop();
	delete m_midiIn;
	delete m_midiOut;
}


void Hwui_01::slot_midiMsgReceived(quint8 *data)
{
	quint8 midiBuffer[3] = {};

	midiBuffer[0] = data[0];    // midi ch, status
	midiBuffer[1] = data[1];    // note/id
	midiBuffer[2] = data[2];    // value

	qDebug() <<Q_FUNC_INFO <<data[0] <<data[1] <<data[2];

	switch( data[1] )
	{
		case  0:	emit signal_button00Pressed(); break;
		case  1:	emit signal_button01Pressed(); break;
		case  2:	emit signal_button02Pressed(); break;
		case  3:	emit signal_button03Pressed(); break;
		case  4:
		{
			m_mute = !m_mute;
			emit signal_button04Pressed(m_mute);
			break;
		}
		case  5:	emit signal_button05Pressed(); break;
		case  6:	emit signal_button06Pressed(); break;
		case  7:	emit signal_button07Pressed(); break;
		case  8:	emit signal_button08Pressed(); break;
		case  9:	emit signal_button09Pressed(); break;
		case 10:	emit signal_button10Pressed(); break;
		case 11:	emit signal_button11Pressed(); break;
	}
}
