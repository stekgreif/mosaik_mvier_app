/**
devices:
0 Sequencer
1 HWUI L
2 HWUI M
3 HWUI R
*/



#ifndef MIDIMANAGER_H
#define MIDIMANAGER_H

#include <core/midi/devices/MidiDevice.h>
#include <core/midi/devices/MidiDevices.h>
#include "MosaikMiniApp.h"
#include "core/midi/mvier/DeviceIdentifier.h"
#include "core/midi/mvier/Hwui_01.h"
#include "core/midi/mvier/Hwui_02.h"
#include "core/midi/mvier/Hwui_03.h"
#include "core/midi/mvier/RgbwButtons.h"


#include <QObject>
#include <QtGlobal>
#include <QMap>



class MidiManager : public QObject
{
    Q_OBJECT

    public:
        static MidiManager &Instance()
        {
            static MidiManager instance;
            return instance;
        }

        void connectFavouriteDevice(void);
        void setStepLed(int i);
        void setParentWidget(MosaikMiniApp *parent);
        void sendData(const QByteArray &data);
        void setChannelPattern(void);
        void refreshPatternView(void);
        void refreshSubchannelSelection(void);
        void setStepsequencerLed(int stepLedId);
        void setMainVolume(float volume);
        void resetHardware(void);
        void subToPreLed(bool state);

    public slots:
        void slot_erpChanged(quint8 id, qint8 val);

    protected:
        MidiManager();
        /*MidiManager(const MidiManager& other)
        {
            //QObject;
            Q_UNUSED(other);
        }*/
        ~MidiManager();

    private:
        //MidiDevice *m_midiDevice;
        MidiDevice *m_midiDevice[10];
		QList<MidiDevices*> m_midiDevicesList;
        bool m_isConnected;
        MosaikMiniApp *m_parent;
		void connectToAllMidiDevices(void);

		QList<DeviceIdentifier*> m_deviceIdentifier;
		QMap<QString, int> m_deviceMap;

		RgbwButtons *m_rgbwButtons;
		Hwui_01 *m_hwui01;
		Hwui_02 *m_hwui02;
		Hwui_03 *m_hwui03;
};

inline MidiManager& midiManager() {return MidiManager::Instance(); }

#endif // MIDIMANAGER_H
