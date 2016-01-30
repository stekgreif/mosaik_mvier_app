#include "MidiDevice.h"
#include "Settings.h"
#include <QDebug>


MidiDevice::MidiDevice()
{
    //qDebug() <<Q_FUNC_INFO <<"Init nothing";
}

MidiDevice::~MidiDevice()
{
    //qDebug() <<"~" <<Q_FUNC_INFO;
}

void MidiDevice::subToPreLed(bool state)
{
    Q_UNUSED(state);
    qDebug() <<Q_FUNC_INFO <<"Not implemented for this Hardware.";
}

