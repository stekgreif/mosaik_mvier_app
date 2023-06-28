#include "DeviceIdentifier.h"
#include <QDebug>
#include <QDateTime>


DeviceIdentifier::DeviceIdentifier()
{
	qDebug() <<Q_FUNC_INFO;
}


DeviceIdentifier::~DeviceIdentifier()
{
	qDebug() <<Q_FUNC_INFO <<m_id;
	delete m_midiIn;
	delete m_midiOut;
}


void DeviceIdentifier::midiConnect(QString hwPort)
{
	m_hwPort = hwPort;
	m_id = -1;

	m_midiOut = new MidiOut(m_hwPort);

	m_midiIn  = new MidiIn(m_hwPort);
	connect(m_midiIn, SIGNAL(signal_midiMsgReceived(quint8*)), this, SLOT(slot_midiMsgReceived(quint8*)) );
	m_midiIn->start();

	/* send ID byte */
	quint8 data[3];
	data[0] = 0xA0;
	data[1] = 0x01;
	data[2] = 0x00;
	m_midiOut->sendDataCopy(data, 3);
}


void DeviceIdentifier::midiScanConnection()
{

}


void DeviceIdentifier::midiDisconnect()
{
	m_midiIn->stop();
	m_midiOut->stop();
	m_hwPort.clear();
}


int DeviceIdentifier::getDeviceId(void)
{
	return m_id;
}


void DeviceIdentifier::slot_midiMsgReceived(quint8 *data)
{
#if 0
	quint8 midiBuffer[3] = {};

	midiBuffer[0] = data[0];    // midi ch, status
	midiBuffer[1] = data[1];    // note/id
	midiBuffer[2] = data[2];    // value
#endif

	qDebug() <<Q_FUNC_INFO <<data[0] <<data[1] <<data[2];

	if( data[0] == 0xA0 )
	{
		qDebug() <<Q_FUNC_INFO <<"m_id is:" <<data[2];
		m_id = data[2];
	}
}

