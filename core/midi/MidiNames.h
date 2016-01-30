#ifndef MIDINAMES
#define MIDINAMES


/* MIDI PORTs: 0..15 */
#define MIDI_CH_SEQ		0
#define MIDI_CH_FNL     1
#define MIDI_CH_FNR		2
#define MIDI_CH_MEN		3
#define MIDI_CH_ERP		4
#define MIDI_CH_PAD		5
#define MIDI_CH_CRF		6
#define MIDI_CH_AHO		7
#define MIDI_CH_AHC		8
#define MIDI_CH_APA		9
#define MIDI_CH_AIN    10
#define MIDI_CH_SPR	   12
#define MIDI_CH_SYS	   15

#define MIDI_MSG_NOTE_ON            0x90
#define MIDI_MSG_NOTE_OFF           0x80
#define MIDI_MSG_CONTROL_CHANGE     0xB0

#endif // MIDINAMES

