#include "MidiInfo.h"
#include <QDebug>
#include "Settings.h"


MidiInfo::MidiInfo()
{
    qDebug() <<Q_FUNC_INFO <<"Init and scan ports.";
    //rescanMidiPorts();
}

MidiInfo::~MidiInfo()
{
    qDebug() <<"~" <<Q_FUNC_INFO;
}



void MidiInfo::rescanMidiPorts()
{
    m_deviceNameList.clear();
    m_devicePortList.clear();
    deviceList();
    qDebug() <<Q_FUNC_INFO <<"elements:" <<m_deviceNameList.size() <<"name:" <<m_deviceNameList <<"port:" <<m_devicePortList;
}

QList<QString> MidiInfo::getDevicePortList(void)
{
    return m_devicePortList;
}

QList<QString> MidiInfo::getDeviceNameList(void)
{
    return m_deviceNameList;
}




void MidiInfo::deviceList(void)
{
    int card, err;

    card = -1;
    if ((err = snd_card_next(&card)) < 0)
    {
        qDebug() <<Q_FUNC_INFO << "cannot determine card number:" << snd_strerror(err);
        return;
    }
    if (card < 0)
    {
        qDebug() <<Q_FUNC_INFO << "no sound card found";
        return;
    }

    //qDebug() << "Device    Name";

    do {
        listCardDevices(card);
        if ((err = snd_card_next(&card)) < 0)
        {
            qDebug() <<Q_FUNC_INFO << "cannot determine card number:" << snd_strerror(err);
            break;
        }
    } while (card >= 0);
}



void MidiInfo::listCardDevices(int card)
{
    snd_ctl_t *ctl;
    char name[32];
    int device;
    int err;

    sprintf(name, "hw:%d", card);

    if ((err = snd_ctl_open(&ctl, name, 0)) < 0)
    {
        qDebug() <<Q_FUNC_INFO << "Cannot open control for card" << card <<":" << snd_strerror(err);
        return;
    }

    device = -1;

    for (;;)
    {
        if ((err = snd_ctl_rawmidi_next_device(ctl, &device)) < 0)
        {
            qDebug() <<Q_FUNC_INFO << "Cannot determine device number:" << snd_strerror(err);
            break;
        }

        if (device < 0)
            break;

        listDevice(ctl, card, device);
    }
    snd_ctl_close(ctl);
}



void MidiInfo::listDevice(snd_ctl_t *ctl, int card, int device)
{
    snd_rawmidi_info_t *info;
    const char *name;
    const char *sub_name;
    int subs;
    int err;

    snd_rawmidi_info_alloca(&info);
    snd_rawmidi_info_set_device(info, device);
    snd_rawmidi_info_set_subdevice(info, 0);
    snd_rawmidi_info_set_stream(info, SND_RAWMIDI_STREAM_OUTPUT);

    if ((err = snd_ctl_rawmidi_info(ctl, info)) < 0 && err != -ENOENT)
    {
        qDebug() <<Q_FUNC_INFO << "Cannot get rawmidi information" << card << ":" << device << ":" << snd_strerror(err);
        return;
    }

    if (err == -ENOENT)
    {
        snd_rawmidi_info_set_stream(info, SND_RAWMIDI_STREAM_INPUT);

        if ((err = snd_ctl_rawmidi_info(ctl, info)) < 0 && err != -ENOENT)
        {
            qDebug() <<Q_FUNC_INFO <<"Cannot get rawmidi information %d:%d: %s" << card << ":" << device << ":" << snd_strerror(err);
            return;
        }
    }

    if (err == -ENOENT)
        return;

    name = snd_rawmidi_info_get_name(info);
    sub_name = snd_rawmidi_info_get_subdevice_name(info);
    subs = snd_rawmidi_info_get_subdevices_count(info);

    if (sub_name[0] == '\0')
    {
        if (subs == 1)
            qDebug() << Q_FUNC_INFO  << "hw:" << card <<"," << device <<"," << name;
        else
            qDebug() << Q_FUNC_INFO << "hw:" << card <<"," << device <<"," << name <<"(" << subs << ")";
    }
    else
    {
        int sub = 0;

        for (;;)
        {
            //qDebug() <<"card:" <<card <<"device:" <<device <<"sub:" <<sub <<endl <<"sub_name:" <<sub_name;

            QString port = "hw:" + QString::number(card) + "," + QString::number(device) + "," + QString::number(sub);
            QString name = QString::fromUtf8(sub_name);

            name = name.split(" ").at(0); //remove 'MIDI 1' part of string

            m_devicePortList.append(port);
            m_deviceNameList.append(name);

            if (++sub >= subs)
                break;

            snd_rawmidi_info_set_subdevice(info, sub);

            if ((err = snd_ctl_rawmidi_info(ctl, info)) < 0)
            {
                qDebug() <<Q_FUNC_INFO <<"Cannot get rawmidi information:" <<card <<"," <<device <<"," <<sub <<"," <<snd_strerror(err);
                break;
            }
            sub_name = snd_rawmidi_info_get_subdevice_name(info);
        }
    }
}

