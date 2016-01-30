#ifndef MIDIPAGE_H
#define MIDIPAGE_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QTextEdit>

#include "core/midi/MidiIn.h"
#include "core/midi/MidiOut.h"

#define MESSAGE_NOTE_ON		0x90
#define MESSAGE_NOTE_OFF 	0x80
#define MESSAGE_CC			0xB0

class PageMidi
    : public QWidget
{
    Q_OBJECT

public:
    PageMidi();
    ~PageMidi();
    void stepUpdate(quint8 step);
    void refresh(void);
public slots:
    void send(void);
    void sendAll(void);
    void parseNewData(quint8* data);
    void printDevices(void);
    void printDevices2(void);
signals:
    void signal_pageButtonPressed(int id);
    void signal_cursorButtonPressed(int id);

protected:
    void paintEvent(QPaintEvent *event);
private:
    QByteArray data;
    MidiIn    *m_midiIn;
    MidiOut   *m_midiOut;
    QSpinBox  *m_inputRgbId;
    QComboBox *m_inputRgbCol;
    QTextEdit *m_midiInTextBox;

    void buttonPressed(int id);
    void device_list(void);
    void list_card_devices(int card);
    void list_device(snd_ctl_t *ctl, int card, int device);

    QString m_midiHwPortName;
    QString m_midiDeviceName;
};

#endif // MIDIPAGE_H
