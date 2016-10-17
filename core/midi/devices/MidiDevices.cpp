#include "MidiDevices.h"
#include <QDebug>
#include "Settings.h"

MidiDevices::MidiDevices()
{

	m_midiOut = new MidiOut(settings().getConnectedMidiDevicePort());

	m_midiIn  = new MidiIn (settings().getConnectedMidiDevicePort());
	connect(m_midiIn, SIGNAL(signal_midiMsgReceived(quint8*)), this, SLOT(slot_midiMsgReceived(quint8*)) );
	m_midiIn->start();
}

MidiDevices::~MidiDevices()
{

}

