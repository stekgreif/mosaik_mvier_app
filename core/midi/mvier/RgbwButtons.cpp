#include "RgbwButtons.h"

#include "core/midi/MidiNames.h"
#include "Settings.h"
#include "MosaikTypes.h"
#include "core/midi/MidiNames.h"
#include "core/subchannel/SubchannelManager.h"

#include <QObject>
#include "QDebug"

#define MIDI_TO_ARDUINO 1
#define MIDI_FROM_ARDUINO 1


#define CMD_SINGLE_COLOR_R	0
#define CMD_SINGLE_COLOR_G	1
#define CMD_SINGLE_COLOR_B	2
#define CMD_SINGLE_COLOR_W	3
#define CMD_COLOR_TABLE		4
#define CMD_UPDATE			5
#define CMD_ALL_LEDS		6
#define CMD_RESET_LEDS		7

uint8_t color_table[6] = {0,1,2,3,4,5};




RgbwButtons::RgbwButtons(QString hwPort)
{
	m_lastStepSequencerLed = 0;
//	qebug() <<Q_FUNC_INFO <<"Get connected midi device" <<settings().getConnectedMidiDevicePort();
	m_midiOut = new MidiOut(hwPort);

	m_midiIn  = new MidiIn (hwPort);
	connect(m_midiIn, SIGNAL(signal_midiMsgReceived(quint8*)), this, SLOT(slot_midiMsgReceived(quint8*)) );
	m_midiIn->start();

	m_lastSubPat.resize(64);

	resetHardware();
}



RgbwButtons::~RgbwButtons()
{
	resetHardware();
}


void RgbwButtons::setStepLed(int i)
{
	qDebug() <<Q_FUNC_INFO <<"send midi";

	QByteArray data;
	data.resize(6);

	int curRelSubCh = subchannelManager().getCurrentSubchannelSelelectionRelative();

	data[0] = MIDI_MSG_NOTE_ON | CMD_COLOR_TABLE;
	data[1] = i;    // led
	data[2] = curRelSubCh;


	data[3] = MIDI_MSG_NOTE_ON | CMD_UPDATE;
	data[4] = 0;
	data[5] = 0;

	m_midiOut->sendData(data);
}



void RgbwButtons::sendRawData(const QByteArray &data)
{

}



void RgbwButtons::setSubchannelPattern()
{

}



void RgbwButtons::setChannelPattern()
{

}



// works only for single subchannel
void RgbwButtons::refreshSequencer()
{
	int curRelSubCh = subchannelManager().getCurrentSubchannelSelelectionRelative(); //0..3

	QBitArray curSubPat;
	curSubPat.resize(64);
	curSubPat = subchannelManager().getCurrentSubchannelPattern();

	QByteArray sendFlags;
	sendFlags.resize(64);

	if( curRelSubCh == m_lastRelSubCh)
	{
		for( int i = 0; i < 64; i++ )
		{
			if(	curSubPat[i] && m_lastSubPat[i] )
				sendFlags[i] = 1;
			else if( !curSubPat[i] && m_lastSubPat[i] )
				sendFlags[i] = -1;
			else if( curSubPat[i] && !m_lastSubPat[i] )
				sendFlags[i] = 1;
			else if( !curSubPat[i] && !m_lastSubPat[i] )
				sendFlags[i] = 0;
		}
	}
	else
	{
		for( int i = 0; i < 64; i++ )
		{
			if(	curSubPat[i] && m_lastSubPat[i] )
				sendFlags[i] = 1;
			else if( !curSubPat[i] && m_lastSubPat[i] )
				sendFlags[i] = -1;
			else if( curSubPat[i] && !m_lastSubPat[i] )
				sendFlags[i] = 1;
			else if( !curSubPat[i] && !m_lastSubPat[i] )
				sendFlags[i] = 0;
		}
	}

	/* load midi send buffer */
	QByteArray sendBuffer;
	for( int i = 0; i < 64; i++ )
	{
		if( 1 == sendFlags[i] )
		{
			sendBuffer.append( (char) (MIDI_MSG_NOTE_ON | CMD_COLOR_TABLE) );
			sendBuffer.append( (char) i );
			sendBuffer.append( (char) curRelSubCh );
		}
		else if( -1 == sendFlags[i] )
		{
			sendBuffer.append( (char) (MIDI_MSG_NOTE_ON | CMD_COLOR_TABLE) );
			sendBuffer.append( (char) i );
			sendBuffer.append( color_table[5] );
		}
	}


	sendBuffer.append( (char) (MIDI_MSG_NOTE_ON | CMD_UPDATE) );
	sendBuffer.append( (char) 0 );
	sendBuffer.append( (char) 0 );

	m_lastRelSubCh = curRelSubCh;
	m_lastSubPat = curSubPat;

	m_midiOut->sendData(sendBuffer);
}



void RgbwButtons::refreshSubchannelSelection()
{

}


void RgbwButtons::resetHardware()
{
	qDebug() <<Q_FUNC_INFO;
	quint8 data[3];
	data[0] = MIDI_MSG_NOTE_ON | CMD_RESET_LEDS;
	data[1] = 0;
	data[2] = 0;
	m_midiOut->sendDataCopy(data, 3);
}



//erstmal ok
void RgbwButtons::setStepsequencerLed(int stepLedId)
{
	qDebug() <<Q_FUNC_INFO <<"send midi";

	QByteArray data;
	data.resize(9);

	QBitArray pattern = subchannelManager().getCurrentSubchannelPattern();
	int color;

	if( pattern.at(m_lastStepSequencerLed) )
		color = color_table[subchannelManager().getCurrentSubchannelSelelectionRelative()];
	else
		color = color_table[5];


	data[0] = MIDI_MSG_NOTE_ON | CMD_COLOR_TABLE;
	data[1] = stepLedId;
	data[2] = color_table[4];

	data[3] = MIDI_MSG_NOTE_ON | CMD_COLOR_TABLE;
	data[4] = m_lastStepSequencerLed;
	data[5] = color;

	data[6] = MIDI_MSG_NOTE_ON | CMD_UPDATE;
	data[7] = 0;
	data[8] = 0;

	m_midiOut->sendData(data);
	m_lastStepSequencerLed = stepLedId;
}



void RgbwButtons::setMainVolume(quint8 volume)
{

}




void RgbwButtons::slot_midiMsgReceived(quint8 *data)
{

		quint8 midiBuffer[3] = {};

		midiBuffer[0] = data[0];    // midi ch, status
		midiBuffer[1] = data[1];    // note/id
		midiBuffer[2] = data[2];    // value

		qDebug() <<Q_FUNC_INFO <<data[0] <<data[1] <<data[2];

		if( data[0] == Mosaik::MidiCommand::noteOn )
		{
			//qDebug() <<Q_FUNC_INFO <<"LABBA";
			emit signal_stepButtonPressed(data[1]);
		}
//		emit signal_seqMsg(data[1], data[2]);
}

