#include "Hwui_03.h"

#include "core/midi/MidiNames.h"
#include "Settings.h"
#include "MosaikTypes.h"

#include <QDebug>




Hwui_03::Hwui_03(QString hwPort)
{

	m_hwPort = hwPort;

	m_midiOut = new MidiOut(m_hwPort);

	m_midiIn  = new MidiIn(m_hwPort);
	connect(m_midiIn, SIGNAL(signal_midiMsgReceived(quint8*)), this, SLOT(slot_midiMsgReceived(quint8*)) );
	m_midiIn->start();

	m_curSubToPre = false;
}


Hwui_03::~Hwui_03()
{
	m_midiIn->stop();
	delete m_midiIn;
	delete m_midiOut;
}


void Hwui_03::slot_midiMsgReceived(quint8 *data)
{
	quint8 midiBuffer[3] = {};

	midiBuffer[0] = data[0];    // midi ch, status
	midiBuffer[1] = data[1];    // note/id
	midiBuffer[2] = data[2];    // value

	qDebug() <<Q_FUNC_INFO <<data[0] <<data[1] <<data[2];


	if( midiBuffer[0] == 0xB0 ) // CC
	{
		if( data[1] == 5 )
		{
			emit signal_button05Pressed(-1*(data[2]-64));
		}

	}
	else if( midiBuffer[0] == 0x90 ) //Note On
	{
		switch( data[1] )
		{
			case  0:	emit signal_button00Pressed(); break;
			case  1:	emit signal_button01Pressed(); break;
			case  2:	emit signal_button02Pressed(); break;
			case  3:	emit signal_button03Pressed(); break;
			case  4:	emit signal_button04Pressed(); break;
			case  5:	emit signal_button05Pressed(1); break;
			case  6:
			{
				m_curSubToPre = !m_curSubToPre;
				qDebug() <<Q_FUNC_INFO <<"m_curSubToPre:" <<m_curSubToPre;
				emit signal_button06Pressed(m_curSubToPre);
				break;
			}
			case  7:	emit signal_button07Pressed(); break;
			case  8:	emit signal_button08Pressed(-1); break;
			case  9:	emit signal_button09Pressed(); break;
			case 10:	emit signal_button10Pressed(-1);	break;
			case 11:	emit signal_button11Pressed(1);		break;
		}
	}





}

