#include "MidiManager.h"

#include <QDebug>
#include <QString>
#include <QObject>

#include "Settings.h"
#include "core/midi/MidiInfo.h"
#include "core/midi/devices/MosaikMini.h"
#include "core/midi/devices/SparkLe.h"
#include "core/midi/devices/VirtualMidiDevice.h"
#include "core/midi/devices/ArduinoTest.h"

#include "core/midi/mvier/Hwui_01.h"
#include "core/midi/mvier/Hwui_02.h"
#include "core/midi/mvier/RgbwButtons.h"
#include "core/midi/mvier/DeviceIdentifier.h"
#include <QDateTime>
#include <QEventLoop>


MidiManager::MidiManager()
{
    qDebug() <<Q_FUNC_INFO <<"Init";
    m_isConnected = false;
    //connectFavouriteDevice();
}


MidiManager::~MidiManager()
{
	qDebug() <<"~" <<Q_FUNC_INFO;
}


void MidiManager::connectToAllMidiDevices()
{
	midiInfo().rescanMidiPorts();
	QList<QString> hwMidiNameList = midiInfo().getDeviceNameList();
	QList<QString> hwMidiPortList = midiInfo().getDevicePortList();

	for( int i = 0; i < hwMidiPortList.size(); i++ )
	{
		m_deviceIdentifier.append(new DeviceIdentifier());
		m_deviceIdentifier.at(i)->midiConnect(hwMidiPortList.at(i));
	}

	/* wait for x ms, non blocking so midi messages can be reveived */
	QEventLoop loop;
	QTimer::singleShot( 250, &loop, SLOT(quit()) );
	loop.exec();

	for( int i = 0; i < hwMidiPortList.size(); i++ )
	{
		m_deviceMap.insert( hwMidiPortList.at(i), m_deviceIdentifier.at(i)->getDeviceId());
		m_deviceIdentifier.at(i)->midiDisconnect();
		m_deviceIdentifier.at(i)->~DeviceIdentifier();
	}
	qDebug() <<Q_FUNC_INFO <<m_deviceMap;
}


void MidiManager::connectFavouriteDevice(void)
{
	connectToAllMidiDevices();
	//qDebug() <<Q_FUNC_INFO <<"Device map at pos 0:" <<m_deviceMap[1];

	for(int i = 0; i < 10; i++)
	{
		if( m_deviceMap.key(i).isEmpty() )
		{
			qDebug() <<Q_FUNC_INFO <<"No matching object to key";
		}
		else
		{
			switch(i)
			{
				case 0:
				{
					m_rgbwButtons = new RgbwButtons( m_deviceMap.key( i ) );
					connect( m_rgbwButtons, SIGNAL(signal_stepButtonPressed(int)),  m_parent, SLOT( slot_stepButtonPressed(int)) );
					break;
				}
				case 1:
				{
					m_hwui01 = new Hwui_01( m_deviceMap.key( i ) );
					connect( m_hwui01, SIGNAL(signal_button10Pressed()),        m_parent, SLOT( slot_sampleLoadToCurrentSubchannel() ) );
					connect( m_hwui01, SIGNAL(signal_button11Pressed()),		m_parent, SLOT( slot_browserSelectedSampleToPrelisten()) );
					connect( m_hwui01, SIGNAL(signal_currentPan(float)),        m_parent, SLOT( slot_parameterPan(float)) );
					connect( m_hwui01, SIGNAL(signal_bpmChanged(float)),        m_parent, SLOT( slot_globalChangeBpmRelative(float)) );
					connect( m_hwui01, SIGNAL(signal_erpChanged(quint8,qint8)),	m_parent, SLOT(slot_erpChanged(quint8,qint8)) );
					connect( m_hwui01, SIGNAL(signal_button08Pressed(bool)),	m_parent, SLOT( slot_parameterMuteAndSolo(bool)) );
					connect( m_hwui01, SIGNAL(signal_button07Pressed()),        m_parent, SLOT( slot_parameterUnmuteAll()) );
					connect( m_hwui01, SIGNAL(signal_button09Pressed()),        m_parent, SLOT( slot_parameterSelectLastMutes()) );
					connect( m_hwui01, SIGNAL(signal_mainVolume(float)),        m_parent, SLOT( slot_globalMainVolume(float)) );
					connect( m_hwui01, SIGNAL(signal_headphoneVolume(float)),   m_parent, SLOT( slot_globalPreVolume(float)) );
					break;
				}
				case 2:
				{
					m_hwui02 = new Hwui_02( m_deviceMap.key( i ) );
					connect( m_hwui02, SIGNAL(signal_erpChanged(quint8,qint8)),	m_parent, SLOT(slot_erpChanged(quint8,qint8)) );
					break;
				}
				case 3:
				{
					m_hwui03 = new Hwui_03( m_deviceMap.key( i ) );
					connect( m_hwui03, SIGNAL(signal_button10Pressed(int)),     m_parent, SLOT( slot_browserChangeCursorPosition(int)) );
					connect( m_hwui03, SIGNAL(signal_button11Pressed(int)),     m_parent, SLOT( slot_browserChangeCursorPosition(int)) );
					connect( m_hwui03, SIGNAL(signal_button05Pressed(int)),		m_parent, SLOT( slot_browserChangeCursorPosition(int)) );
					connect( m_hwui03, SIGNAL(signal_button08Pressed(int)),		m_parent, SLOT( slot_browserChangeCursorPosition(int)) );
					connect( m_hwui03, SIGNAL(signal_button07Pressed()),		m_parent, SLOT( slot_browserCloseFolder() ) );
					connect( m_hwui03, SIGNAL(signal_button09Pressed()),		m_parent, SLOT( slot_browserOpenFolder() ) );
					connect( m_hwui03, SIGNAL(signal_button04Pressed()),        m_parent, SLOT( slot_prelistenSubchannelSample()) );
					connect( m_hwui03, SIGNAL(signal_button06Pressed(bool)),    m_parent, SLOT( slot_parameterCurrentSubToPre(bool)) );
					break;
				}
				default:
					break;
			}
		}
	}
}




#if 0 // 2016-08-28 new version: should connect with more than one midi device
void MidiManager::connectFavouriteDevice(void)
{
    qDebug() <<Q_FUNC_INFO <<"New MIDI connector function";

    midiInfo().rescanMidiPorts();


    QList<QString> hwMidiNameList = midiInfo().getDeviceNameList();
    QList<QString> hwMidiPortList = midiInfo().getDevicePortList();
    QList<QString> favMidiDevList = settings().getFavouriteMidiDeviceList();
    qDebug() <<Q_FUNC_INFO <<"Number of MIDI Devices:" <<hwMidiPortList.size();


	int midiHwDevice = 0;


	if( hwMidiPortList.size() > 0 )
    {
        qDebug() <<Q_FUNC_INFO <<"Favourite device is:" <<hwMidiNameList.at(midiHwDevice);
        settings().setConnectedMidiDeviceName(hwMidiNameList.at(midiHwDevice));
        settings().setConnectedMidiDevicePort(hwMidiPortList.at(midiHwDevice));
        m_midiDevice[0] = new ArduinoTest();
        connect( m_midiDevice[0], SIGNAL(signal_stepButtonPressed(int)),  m_parent, SLOT( slot_stepButtonPressed(int)) );
    }
	else
	{
		qDebug() <<Q_FUNC_INFO <<"Connected to virtual";
		settings().setConnectedMidiDeviceName("virtual");
		settings().setConnectedMidiDevicePort("virtual");
		m_midiDevice[0] = new VirtualMidiDevice();
	}

    m_isConnected = true;
}
#endif




#if 0 // 2016-08-28 working version
void MidiManager::connectFavouriteDevice(void)
{
    qDebug() <<Q_FUNC_INFO;

    midiInfo().rescanMidiPorts();


    QList<QString> hwMidiNameList = midiInfo().getDeviceNameList();
    QList<QString> hwMidiPortList = midiInfo().getDevicePortList();
    QList<QString> favMidiDevList = settings().getFavouriteMidiDeviceList();

    int midiHwDevice = -1;

    for(int fav = 0; (fav < favMidiDevList.size()) && (midiHwDevice == -1); fav++ )
    {
        //qDebug() <<"fav" <<fav;
        for(int dev = 0; (dev < hwMidiNameList.size()) && (midiHwDevice == -1); dev++ )
        {
            //qDebug() <<"dev" <<dev;
            if( hwMidiNameList.at(dev) == favMidiDevList.at(fav) )
            {
                midiHwDevice = dev;
                qDebug() <<Q_FUNC_INFO <<"midiHwDevice: " <<midiHwDevice;
            }
        }
    }

    if( m_isConnected )
        delete m_midiDevice;


    if( midiHwDevice == -1 )
    {
        qDebug() <<Q_FUNC_INFO <<"Connected to virtual";
        settings().setConnectedMidiDeviceName("virtual");
        settings().setConnectedMidiDevicePort("virtual");
        m_midiDevice = new VirtualMidiDevice();
    }
    else
    {
        qDebug() <<Q_FUNC_INFO <<"Favourite device is:" <<hwMidiNameList.at(midiHwDevice);
        settings().setConnectedMidiDeviceName(hwMidiNameList.at(midiHwDevice));
        settings().setConnectedMidiDevicePort(hwMidiPortList.at(midiHwDevice));

        if( hwMidiNameList.at(midiHwDevice) == "MOSAIKmini" )
        {
            m_midiDevice = new MosaikMini();

            /** app **/
            connect( m_midiDevice, SIGNAL(signal_functionLeftButton00Pressed()),         m_parent, SLOT( slot_appToggleFullScreen()) );
            connect( m_midiDevice, SIGNAL(signal_functionLeftButton01Pressed()),         m_parent, SLOT( slot_appExit()) );

            /** selection **/
            connect( m_midiDevice, SIGNAL(signal_functionSelectSubchannelRelative(int)), m_parent, SLOT( slot_selectionSetCurrentSubchannelRelative(int)) );
            connect( m_midiDevice, SIGNAL(signal_selectChannel(int)),                    m_parent, SLOT( slot_selectionSetCurrentChannel(int) ) );
            connect( m_midiDevice, SIGNAL(signal_selectionSubchannelNext()),             m_parent, SLOT( slot_selectionNextSubchannel() ));

            /** browser **/
            connect( m_midiDevice, SIGNAL(signal_encChanged(int)),                       m_parent, SLOT( slot_browserChangeCursorPosition(int)) );
            connect( m_midiDevice, SIGNAL(signal_menuNavigation(int)),                   m_parent, SLOT( slot_browserChangeCursorPosition(int)) );
            connect( m_midiDevice, SIGNAL(signal_menuEncoderPushed()),                   m_parent, SLOT( slot_browserToggleFolderExpansion()) );
            connect( m_midiDevice, SIGNAL(signal_setPathId(int)),                        m_parent, SLOT( slot_browserChangePathId(int)) );
            connect( m_midiDevice, SIGNAL(signal_browserOpenFolder()),                   m_parent, SLOT( slot_browserOpenFolder()) );
            connect( m_midiDevice, SIGNAL(signal_browserCloseFolder()),                  m_parent, SLOT( slot_browserCloseFolder()) );

            /** prelisten **/
            connect( m_midiDevice, SIGNAL(signal_prelistenBrowserSample()),              m_parent, SLOT( slot_browserSelectedSampleToPrelisten()) );
            connect( m_midiDevice, SIGNAL(signal_prelistenSubchannelSample()),           m_parent, SLOT( slot_prelistenSubchannelSample()) );

            /** sequencer/pattern **/
            connect( m_midiDevice, SIGNAL(signal_stepButtonPressed(int)),                m_parent, SLOT( slot_stepButtonPressed(int)) );
            connect( m_midiDevice, SIGNAL(signal_functionRightButton04Pressed()),        m_parent, SLOT( slot_patternClearAll()) );
            connect( m_midiDevice, SIGNAL(signal_functionRightButton06Pressed()),        m_parent, SLOT( slot_patternClearCurrentChannel()) );

            /** main parameter **/
            connect( m_midiDevice, SIGNAL(signal_bpmChanged(float)),                     m_parent, SLOT( slot_globalChangeBpmRelative(float)) );
            connect( m_midiDevice, SIGNAL(signal_mainVolume(float)),                     m_parent, SLOT( slot_globalMainVolume(float)) );
            connect( m_midiDevice, SIGNAL(signal_headphoneVolume(float)),                m_parent, SLOT( slot_globalPreVolume(float)) );

            /** sample **/
            connect( m_midiDevice, SIGNAL(signal_functionRightButton00Pressed()),        m_parent, SLOT( slot_sampleUnloadAll() ) );
            connect( m_midiDevice, SIGNAL(signal_functionRightButton02Pressed()),        m_parent, SLOT( slot_sampleUnloadCurrentChannel() ) );
            connect( m_midiDevice, SIGNAL(signal_loadSample()),                          m_parent, SLOT( slot_sampleLoadToCurrentSubchannel() ) );

            /** tile **/
            connect( m_midiDevice, SIGNAL(signal_functionLeftButton03Pressed()),         m_parent, SLOT( slot_patternClearCurrentSubchannel()) );
            connect( m_midiDevice, SIGNAL(signal_functionLeftButton03Pressed()),         m_parent, SLOT( slot_sampleUnloadCurrentSubchannel() ) );


            /** subchannel parameter **/
            connect( m_midiDevice, SIGNAL(signal_currentPan(float)),                     m_parent, SLOT( slot_parameterPan(float)) );
            connect( m_midiDevice, SIGNAL(signal_playDirection(bool)),                   m_parent, SLOT( slot_parameterPlayDirection(bool)) );
            connect( m_midiDevice, SIGNAL(signal_currentSubchannelToPre(bool)),          m_parent, SLOT( slot_parameterCurrentSubToPre(bool)) );
            connect( m_midiDevice, SIGNAL(signal_muteAndSolo(bool)),                     m_parent, SLOT( slot_parameterMuteAndSolo(bool)) );
            connect( m_midiDevice, SIGNAL(signal_unmuteAll()),                           m_parent, SLOT( slot_parameterUnmuteAll()) );
            connect( m_midiDevice, SIGNAL(signal_lastMute()),                            m_parent, SLOT( slot_parameterSelectLastMutes()) );

            //connect( m_midiDevice, SIGNAL(signal_encChanged(float)),                     m_parent, SLOT(slot_subchannelChangeVolume(float)) );
            connect( m_midiDevice, SIGNAL(signal_erpChanged(quint8,qint8)),              m_parent, SLOT(slot_erpChanged(quint8,qint8)) );
            connect( m_midiDevice, SIGNAL(signal_selectPageSubchannel()),                m_parent, SLOT(slot_uiSetToPageSubchannel()) );
            connect( m_midiDevice, SIGNAL(signal_selectPageBrowser()),                   m_parent, SLOT(slot_uiSetToPageBrowser()) );


        }
        else if( hwMidiNameList.at(midiHwDevice) == "MOSAIK" )
        {
            //m_midiDevice = new Mosaik;
        }
        else if( hwMidiNameList.at(midiHwDevice) == "SparkLE" )
        {
            m_midiDevice = new SparkLe();
            connect( m_midiDevice, SIGNAL(signal_stepButtonPressed(int)), m_parent, SLOT(slot_stepButtonPressed(int)) );
            connect( m_midiDevice, SIGNAL(signal_erpChanged(quint8,qint8)), m_parent, SLOT(slot_erpChanged(quint8,qint8)) );

            connect( m_midiDevice, SIGNAL(signal_bpmChanged(float)),           m_parent, SLOT(slot_globalChangeBpmRelative(float)) );
            //connect( m_midiDevice, SIGNAL(signal_mainVolume(quint8)),          m_parent, SLOT(slot_setMainVolume(quint8)) );
            connect( m_midiDevice, SIGNAL(signal_currentSubchannelVol(float)), m_parent, SLOT(slot_parameterChangeCurrentSubchVolume(float)) );

            connect( m_midiDevice, SIGNAL(signal_currentSubchannelPanRel(float)), m_parent, SLOT(slot_parameterPan(float)) );
        }
        else if( hwMidiNameList.at(midiHwDevice) == "UNTZtrument" )
        {
            //m_midiDevice = new Untztrument;
        }
        else if ( hwMidiNameList.at(midiHwDevice) == "Arduino" )
        {
            m_midiDevice = new ArduinoTest();

            /** sequencer/pattern **/
            connect( m_midiDevice, SIGNAL(signal_stepButtonPressed(int)),                m_parent, SLOT( slot_stepButtonPressed(int)) );
        }
        else
        {
            qDebug() <<Q_FUNC_INFO <<"Device not in List!";
        }
    }

    m_isConnected = true;
}

#endif


void MidiManager::setStepLed(int i)
{
	m_rgbwButtons->setStepLed(i);
}


void MidiManager::setParentWidget(MosaikMiniApp *parent)
{
    m_parent = parent;
}


void MidiManager::slot_erpChanged(quint8 id, qint8 val)
{
    qDebug() <<Q_FUNC_INFO <<"Midi Received:" <<"id:" <<id <<"val:" <<val;
    m_parent->slot_erpChanged(id, val);
}


void MidiManager::sendData(const QByteArray &data)
{
    qDebug() << Q_FUNC_INFO << data.size();
	m_rgbwButtons->sendRawData(data);
}


void MidiManager::setChannelPattern()
{
	m_rgbwButtons->setChannelPattern();
}


void MidiManager::refreshPatternView()
{
	m_rgbwButtons->refreshSequencer();
}


void MidiManager::refreshSubchannelSelection()
{
	m_rgbwButtons->refreshSubchannelSelection();
}


void MidiManager::setStepsequencerLed(int stepLedId)
{
	m_rgbwButtons->setStepsequencerLed(stepLedId);
}


void MidiManager::setMainVolume(float volume)
{
    int vol = 127 - (127 * volume);
    qDebug() <<Q_FUNC_INFO <<"vol" <<vol;
	//m_midiDevice[0]->setMainVolume(vol);
}


void MidiManager::resetHardware()
{
	//m_rgbwButtons->resetHardware();
}


void MidiManager::subToPreLed(bool state)
{
    Q_UNUSED(state);
	//m_midiDevice[0]->subToPreLed(state);
}
