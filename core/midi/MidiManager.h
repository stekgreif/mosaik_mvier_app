#ifndef MIDIMANAGER_H
#define MIDIMANAGER_H

#include <core/midi/devices/MidiDevice.h>
#include "MosaikMiniApp.h"

#include <QObject>
#include <QtGlobal>



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
        MidiManager(const MidiManager& other)
        {
            //QObject;
            Q_UNUSED(other);
        }
        ~MidiManager();

    private:
        MidiDevice *m_midiDevice;
        bool m_isConnected;
        MosaikMiniApp *m_parent;
};

inline MidiManager& midiManager() {return MidiManager::Instance(); }

#endif // MIDIMANAGER_H
