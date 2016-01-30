#ifndef MIDIINPARSER_H
#define MIDIINPARSER_H

#include "MidiIn.h"
#include <QObject>
#include <QWidget>

class MidiInParser : public QWidget
{
    Q_OBJECT
public:
    MidiInParser();
    ~MidiInParser();

private:
    MidiIn *m_midiIn;

public slots:
    void slot_midiMsgReceived(quint8* data);

signals:
    void signal_seqMsg(quint8 id, quint8 val);
    void signal_fnlMsg(quint8 id, quint8 val);
    void signal_fnrMsg(quint8 id, quint8 val);
    void signal_menMsg(quint8 id, quint8 val);

    void signal_erpMsg(quint8 id, quint8 val);
    void signal_padMsg(quint8 id, quint8 val);
    void signal_crfMsg(quint8 id, quint8 val);
    void signal_ahoMsg(quint8 id, quint8 val);

    void signal_ahcMsg(quint8 id, quint8 val);
    void signal_apaMsg(quint8 id, quint8 val);
    void signal_ainMsg(quint8 id, quint8 val);
    void signal_sprMsg(quint8 id, quint8 val);

    void signal_sysMsg(quint8 id, quint8 val);

    void signal_sparkMsg(quint8 id, int sigVal);
};

#endif // MIDIINPARSER_H
