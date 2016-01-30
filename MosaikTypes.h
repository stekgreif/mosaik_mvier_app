#ifndef MOSAIKTYPES_H
#define MOSAIKTYPES_H


typedef struct {
    float start;
    float fadeIn;
    float fadeOut;
    float end;
} envelope_t;



namespace Mosaik
{
    namespace MidiCommand
    {
        enum midiCommand
        {
            noteOff = 0x80,
            noteOn = 0x90,
            controlChange = 0xB0
        };
    }

    namespace Functions
    {
        enum functions
        {
            clearCurrentSubchannelPattern,
            clearCurrentChannelPattern
        };
    }

    struct Pages
    {
        enum pages
        {
            Subchannel = 0,
            Browser = 1
        };
    };

#if 1
    struct MidiChannels
    {
        enum midiChannels
        {
            Seq =  0,
            Fnl =  1,
            Fnr =  2,
            Men =  3,
            Erp =  4,
            Pad =  5,
            Crf =  6,
            Aho =  7,
            Ahc =  8,
            Apa =  9,
            Ain = 10,
            Spr = 12,
            Sys = 15
        };
    };
#endif



    struct SparkLeCc
    {
        enum sparkLeCc
        {
            Tempo = 48,
            Volume = 47,
            P1 = 49,
            P2 = 50,
            P3 = 51,
            Divide = 52,
            Move = 53
        };
    };

#if 0
    struct Midi
    {
        struct Channels
        {
            enum MosaikMiniDevice
            {
                Seq =  0,
                Fnl =  1,
                Fnr =  2,
                Men =  3,
                Erp =  4,
                Pad =  5,
                Crf =  6,
                Aho =  7,
                Ahc =  8,
                Apa =  9,
                Ain = 10,
                Spr = 12,
                Sys = 15
            };
        };
    };
#endif

}




namespace MosaikMiniDevice {
    enum MidiChannel {
        MidiChannelSeq =  0,
        MidiChannelFnl =  1,
        MidiChannelFnr =  2,
        MidiChannelMen =  3,
        MidiChannelErp =  4,
        MidiChannelPad =  5,
        MidiChannelCrf =  6,
        MidiChannelAho =  7,
        MidiChannelAhc =  8,
        MidiChannelApa =  9,
        MidiChannelAin = 10,
        MidiChannelSpr = 12,
        MidiChannelSys = 15
    };

    enum RgbColors {
        RgbColorOff     = 0xF,
        RgbColorRed     = 0xD,
        RgbColorGreen   = 0xB,
        RgbColoryellow  = 0x9,
        RgbColorblue    = 0x7,
        RgbColormagenta = 0x5,
        RgbColorcyan    = 0x3,
        RgbColorwhite   = 0x1
    };
}


#endif // MOSAIKTYPES_H

