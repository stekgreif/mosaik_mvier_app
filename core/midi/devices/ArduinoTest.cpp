#include "ArduinoTest.h"

#include "MosaikMini.h"
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



ArduinoTest::ArduinoTest()
{
	m_lastStepSequencerLed = 0;
    qDebug() <<Q_FUNC_INFO <<"Get connected midi device" <<settings().getConnectedMidiDevicePort();
    m_midiOut = new MidiOut(settings().getConnectedMidiDevicePort());

    m_midiIn  = new MidiIn (settings().getConnectedMidiDevicePort());
    connect(m_midiIn, SIGNAL(signal_midiMsgReceived(quint8*)), this, SLOT(slot_midiMsgReceived(quint8*)) );
    m_midiIn->start();
}



ArduinoTest::~ArduinoTest()
{

}


//ok
void ArduinoTest::setStepLed(int i)
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



void ArduinoTest::sendRawData(const QByteArray &data)
{

}



void ArduinoTest::setSubchannelPattern()
{

}



void ArduinoTest::setChannelPattern()
{

}



// works only for single subchannel
void ArduinoTest::refreshSequencer()
{
	int curRelSubCh = subchannelManager().getCurrentSubchannelSelelectionRelative(); //0..3

	QBitArray curSubPat = subchannelManager().getCurrentSubchannelPattern();
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
				sendFlags[i] = 0;
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

	m_lastRelSubCh = curRelSubCh;
	m_lastSubPat = curSubPat;

	m_midiOut->sendData(sendBuffer);
}






//TODO
#if 0 // 2016-09-30 working old version
void ArduinoTest::refreshSequencer()
{
    qDebug() <<Q_FUNC_INFO;

    QBitArray pattern = subchannelManager().getCurrentSubchannelPattern();
    int currentRelativeSubchannel = subchannelManager().getCurrentSubchannelSelelectionRelative();

    QByteArray midiData;
    midiData.resize(192);

    //qDebug() <<Q_FUNC_INFO <<"midi bytes to send:" <<midiData.size();

    for (int i = 0; i < midiData.size()/3; i++)
    {
        // 0 is color
        midiData[3*i+1] = i;    // led

        midiData[3*i+2] = 100;   // brightness


        if( pattern.at(i) )
        {
            switch (currentRelativeSubchannel)
            {
                case 0:
                    midiData[3*i+0] = MIDI_MSG_CONTROL_CHANGE | 0;
                    break;
                case 1:
                    midiData[3*i+0] = MIDI_MSG_CONTROL_CHANGE | 1;
                    break;
                case 2:
                    midiData[3*i+0] = MIDI_MSG_CONTROL_CHANGE | 2;
                    break;
                case 3:
                    midiData[3*i+0] = MIDI_MSG_CONTROL_CHANGE | 3;
                    break;
                default:
                    break;
            }
        }
        else
            midiData[3*i+0] = 0;
    }

    QByteArray midiSendHelper;
    midiSendHelper = midiData;
    midiSendHelper.resize(32);


    m_midiOut->sendData(midiSendHelper);
    //qDebug() <<Q_FUNC_INFO <<"Midi Bytes to send:" <<midiData.size();
}
#endif



void ArduinoTest::refreshSubchannelSelection()
{

}


//ok
void ArduinoTest::resetHardware()
{
    qDebug() <<Q_FUNC_INFO;
    quint8 data[3];
	data[0] = MIDI_MSG_NOTE_ON | CMD_RESET_LEDS;
	data[1] = 0;
	data[2] = 0;
    m_midiOut->sendDataCopy(data, 3);
}



//erstmal ok
void ArduinoTest::setStepsequencerLed(int stepLedId)
{
	qDebug() <<Q_FUNC_INFO <<"send midi";

	QByteArray data;
	data.resize(9);

	data[0] = MIDI_MSG_NOTE_ON | CMD_COLOR_TABLE;
	data[1] = stepLedId;
	data[2] = color_table[1];

	data[3] = MIDI_MSG_NOTE_ON | CMD_COLOR_TABLE;
	data[4] = m_lastStepSequencerLed;
	data[5] = color_table[0];

    m_lastStepSequencerLed = stepLedId;
}



void ArduinoTest::setMainVolume(quint8 volume)
{

}



void ArduinoTest::slot_midiMsgReceived(quint8 *data)
{
        quint8 midiBuffer[3] = {};

        midiBuffer[0] = data[0];    // midi ch, status
        midiBuffer[1] = data[1];    // note/id
        midiBuffer[2] = data[2];    // value

        qDebug() <<Q_FUNC_INFO <<data[0] <<data[1] <<data[2];

        if( data[0] == Mosaik::MidiCommand::noteOn )
        {
            qDebug() <<Q_FUNC_INFO <<"LABBA";
            emit signal_stepButtonPressed(data[1]);
        }
        emit signal_seqMsg(data[1], data[2]);
}
