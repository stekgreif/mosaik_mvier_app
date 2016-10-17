#ifndef MIDIINFO_H
#define MIDIINFO_H

#include <alsa/asoundlib.h>
#include <QString>
#include <QList>


typedef struct {
    QString hwPort;
    QString name;
} midiDevice_t;

class MidiInfo
{
public:
    static MidiInfo &Instance()
    {
        static MidiInfo instance;
        return instance;
    }

    QList<QString> getDeviceNameList(void);
    QList<QString> getDevicePortList(void);
	QList<int> getDevicePortListInt(void);

    void rescanMidiPorts(void);

protected:
    MidiInfo();
    MidiInfo(const MidiInfo& other) {Q_UNUSED(other);}
    ~MidiInfo();

private:
    void deviceList(void);
    void listCardDevices(int card);
    void listDevice(snd_ctl_t *ctl, int card, int device);

    QList <QString> m_deviceNameList;
    QList <QString> m_devicePortList;
	QList <int> m_devicePortListInt;
};

inline MidiInfo& midiInfo() { return MidiInfo::Instance(); }

#endif // MIDIINFO_H
