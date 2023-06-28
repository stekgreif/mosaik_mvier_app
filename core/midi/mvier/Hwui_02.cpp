#include "Hwui_02.h"
#include "core/midi/MidiNames.h"
#include "Settings.h"
#include "MosaikTypes.h"
#include <QDebug>
#include <QString>


Hwui_02::Hwui_02(QString hwPort)
{
	m_hwPort = hwPort;
	m_midiOut = new MidiOut(m_hwPort);
	m_midiIn  = new MidiIn(m_hwPort);
	connect(m_midiIn, SIGNAL(signal_midiMsgReceived(quint8*)), this, SLOT(slot_midiMsgReceived(quint8*)) );
	m_midiIn->start();
}


Hwui_02::~Hwui_02()
{
	m_midiIn->stop();
	delete m_midiIn;
	delete m_midiOut;
}


void Hwui_02::slot_midiMsgReceived(quint8 *data)
{
    /*  0: midi ch, status
        1: note/id
        2: value */
	qDebug() <<Q_FUNC_INFO <<data[0] <<data[1] <<data[2];

	switch( data[1] )
	{
		case  0:
		case  1:
		case  2:
		case  3:
		{
			qint8 diff = data[2] - 64;
			emit signal_erpChanged(data[1], diff);
			break;
		}
        default: break;
	}
}
