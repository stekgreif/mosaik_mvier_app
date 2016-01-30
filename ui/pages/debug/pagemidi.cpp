#include "pagemidi.h"
#include <QWidget>
#include <QPushButton>
#include <QPainter>
#include <QStyleOption>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QDebug>
#include <QTextEdit>
#include <alsa/asoundlib.h>

/// @todo 22 - move this stuff to PageDebug
///
///
#define DISABLE_MIDI

PageMidi::PageMidi()
    : QWidget()
    , m_midiInTextBox(new QTextEdit(this))
{
#ifndef DISABLE_MIDI
    /* MIDI */
    device_list();

    if( m_midiDeviceName.contains("MOSAIK MINI"))
    {
        m_midiIn = new MidiIn(m_midiHwPortName);
        m_midiOut = new MidiOut(m_midiHwPortName);
        qDebug() <<Q_FUNC_INFO << "Connected to Mosaik";
    }
    else
    {
        m_midiIn = new MidiIn();
        m_midiOut = new MidiOut();
    }
#endif

    /* UI */
    this->setObjectName("pageMidi");


    QLabel *headline = new QLabel("Audio Midi Debug Page", this);
    headline->setObjectName("pageheadline");
    headline->move(40, 40);

    QPushButton *btn1 = new QPushButton(this);
    btn1->move(310,300);
    btn1->setFixedHeight(100);
    btn1->setText("Send Data");

    QPushButton *btnSendAll = new QPushButton(this);
    btnSendAll->move(630,300);
    btnSendAll->setFixedHeight(100);
    btnSendAll->setText("Send Color To All");

    m_inputRgbId = new QSpinBox(this);
    m_inputRgbId->move(180, 300);
    m_inputRgbId->setMaximum(63);
    m_inputRgbId->setFixedHeight(100);
    m_inputRgbId->setFixedWidth(100);
    m_inputRgbId->setStyleSheet("QSpinBox:up-button   {width: 50px;}"
                                "QSpinBox:down-button {width: 50px;}");

    m_inputRgbCol = new QComboBox(this);
    m_inputRgbCol->setObjectName("inputRgbColor");
    m_inputRgbCol->setStyleSheet("QComboBox#inputRgbColor {font-size: 20pt;}");
    m_inputRgbCol->move(420, 300);
    m_inputRgbCol->setFixedHeight(100);
    m_inputRgbCol->setFixedWidth(200);
    m_inputRgbCol->insertItem(0, "- off -", 15);
    m_inputRgbCol->insertItem(1, "white", 1);
    m_inputRgbCol->insertItem(2, "cyan", 3);
    m_inputRgbCol->insertItem(3, "magenta", 5);
    m_inputRgbCol->insertItem(4, "blue", 7);
    m_inputRgbCol->insertItem(5, "yellow", 9);
    m_inputRgbCol->insertItem(6, "green", 11);
    m_inputRgbCol->insertItem(7, "red", 13);
    m_inputRgbCol->insertItem(8, "- off -", 15);


    m_midiInTextBox->setObjectName("midiInTextBox");
    m_midiInTextBox->setReadOnly(true);
    m_midiInTextBox->setStyleSheet("QTextEdit#midiInTextBox {background-color rgb(100,100,100);}");
    m_midiInTextBox->setFontFamily("Droid Sans Mono");
    m_midiInTextBox->move(200,600);
    m_midiInTextBox->setFixedSize(600,200);


#ifndef DISABLE_MIDI
    connect(btn1, SIGNAL(clicked()), this, SLOT(send()));
    connect(btnSendAll, SIGNAL(clicked()), this, SLOT(sendAll()));

    /* MIDI */
    m_midiIn->start();
    connect(m_midiIn, SIGNAL(received(quint8*)), this, SLOT(parseNewData(quint8*)));

    //printDevices2();
    device_list();
#endif
}



PageMidi::~PageMidi()
{

}



///
void PageMidi::printDevices(void)
{
    char **hints;

    /* Enumerate sound devices */
    int err = snd_device_name_hint(-1, "pcm", (void***)&hints);

    if (err != 0)
    {
       return; //Error!
    }

    char** n = hints;


    while (*n != NULL)
    {
        char *name = snd_device_name_get_hint(*n, "NAME");
        char *desc = snd_device_name_get_hint(*n, "DESC");
        char *ioid = snd_device_name_get_hint(*n, "IOID");

        if (name != NULL && 0 != strcmp("null", name))
        {
            m_midiInTextBox->append(  QString::fromUtf8(name)
                                    + "  ---  "
                                    + QString::fromUtf8(desc)
                                    + "  ---  "
                                    + QString::fromUtf8(ioid));
            free(name);
        }
        n++;
    }

    snd_device_name_free_hint((void**)hints);  //Free hint buffer too
}




void PageMidi::printDevices2(void)
{
    snd_seq_client_info_t *cinfo;
    snd_seq_port_info_t *pinfo;
    int count;
    snd_seq_t *seq;

    if (snd_seq_open(&seq, "default", SND_SEQ_OPEN_DUPLEX, 0) < 0)
    {
        qDebug() <<Q_FUNC_INFO << "Can't open sequencer\n";
        return;
    }

    snd_seq_client_info_alloca(&cinfo);
    snd_seq_port_info_alloca(&pinfo);
    snd_seq_client_info_set_client(cinfo, -1);

    while (snd_seq_query_next_client(seq, cinfo) >= 0)
    {
        /* reset query info */
        snd_seq_port_info_set_client(pinfo, snd_seq_client_info_get_client(cinfo));
        snd_seq_port_info_set_port(pinfo, -1);
        count = 0;

        while (snd_seq_query_next_port(seq, pinfo) >= 0)
        {
#if 0
            if (check_permission(pinfo, perm))
            {
                do_action(seq, cinfo, pinfo, count);
                count++;
            }
            qDebug() << "ID:"    << snd_seq_client_info_get_client(cinfo) << " - "
                     << "NAME: " << snd_seq_client_info_get_name(cinfo) << " - "
                     << "TYPE:"  << snd_seq_client_info_get_type(cinfo) << " - "
                     << "NUM PORTS" << snd_seq_client_info_get_num_ports(cinfo);
#endif
            count++;

        }
    }


    snd_seq_close(seq);
}


void PageMidi::parseNewData(quint8* data)
{
    QString midiMsgType;
    if( (data[0] & 0b11110000) == 0x80)
        midiMsgType = "NOTE OFF  ";
    else if( (data[0] & 0b11110000) == 0x90)
        midiMsgType = "NOTE ON   ";
    else if( (data[0] & 0b11110000) == 0xB0)
        midiMsgType = "CTR CHAN  ";
    else
        midiMsgType = "unknown   ";

    m_midiInTextBox->append(  midiMsgType
                            + QString("%1").arg( data[0] & 0b00001111, 2, 10, QChar('0') )
                            + "  "
                            + QString("%1").arg( data[1], 3, 10, QChar('0') )
                            + "  "
                            + QString("%1").arg( data[2], 3, 10, QChar('0') ));

    if (((data[0] & 0xF0) == 0x90) && ((data[0] & 0x0F) == 0x01) )
    {
        emit signal_pageButtonPressed(data[1]);
    }

    if (((data[0] & 0xF0) == 0x90) && ((data[0] & 0x0F) == 0x03) )
    {
        emit signal_cursorButtonPressed(data[1]);
    }
}



void PageMidi::send(void)
{
#ifndef DISABLE_MIDI
    data[0] = MESSAGE_NOTE_ON;
    data[1] = m_inputRgbId->text().toInt();
    data[2] = m_inputRgbCol->itemData(m_inputRgbCol->currentIndex()).toInt();
    m_midiOut->sendData(data);
#endif
}


void PageMidi::sendAll(void)
{
#ifndef DISABLE_MIDI
    data[0] = MESSAGE_NOTE_ON;
    data[1] = 64;
    data[2] = m_inputRgbCol->itemData(m_inputRgbCol->currentIndex()).toInt();
    m_midiOut->sendData(data);
#endif

#if 0  // works only with 63 elements
#define MIDI_MSGS 63
    quint8 datyArray[MIDI_MSGS*3];

    quint8 color = m_inputRgbCol->itemData(m_inputRgbCol->currentIndex()).toInt();
    for(int cnt = 0; cnt < MIDI_MSGS; cnt++)
    {
        datyArray[(cnt*3)+0] = MESSAGE_NOTE_ON;
        datyArray[(cnt*3)+1] = cnt;
        datyArray[(cnt*3)+2] = color;
    }
    m_midiOut->sendDataCopy(datyArray, (MIDI_MSGS*3));
#endif
}




void PageMidi::stepUpdate(quint8 step)
{
    Q_UNUSED(step);
#ifndef DISABLE_MIDI
    data[0] = MESSAGE_NOTE_ON;
    data[1] = 64;
    data[2] = 15;

    data[3] = MESSAGE_NOTE_ON;
    data[4] = step;
    data[5] = 7;
    m_midiOut->sendData(data);
#endif
}

void PageMidi::refresh()
{

}




/////////////////////////////////////////////////////////////////////

/* call this function to populate m_midiDeviceName and m_midiHwPort*/
///@todo 45 - this is only for ONE midi device
/// @todo midi detection has to move into the midi c
void PageMidi::device_list(void)
{
#ifndef DISABLE_MIDI
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
        list_card_devices(card);
        if ((err = snd_card_next(&card)) < 0)
        {
            qDebug() <<Q_FUNC_INFO << "cannot determine card number:" << snd_strerror(err);
            break;
        }
    } while (card >= 0);
#endif
}




void PageMidi::list_card_devices(int card)
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

        list_device(ctl, card, device);
    }
    snd_ctl_close(ctl);
}



void PageMidi::list_device(snd_ctl_t *ctl, int card, int device)
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
            //qDebug() << "MIDI HW:" << card <<"," << device << "," << sub <<"," << sub_name;


            m_midiHwPortName =  "hw:"
                            + QString::number(card)
                            + ","
                            + QString::number(device)
                            + ","
                            + QString::number(sub);
            m_midiDeviceName = QString::fromUtf8(sub_name);

            //qDebug() <<Q_FUNC_INFO << m_midiDeviceName << m_midiHwPortName;

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




void PageMidi::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption myOption;
    myOption.initFrom(this);
    QPainter myPainter(this);
    style()->drawPrimitive( QStyle::PE_Widget, &myOption, &myPainter,this);
}
