#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSize>
#include <QList>



#define SETTINGS_INIT_WITH_AUDIO        true
#define SETTINGS_INIT_WITH_MIDI_IN      false
#define SETTINGS_INIT_WITH_MIDI_OUT     false
#define SETTINGS_START_WITH_FULL_SCREEN true


#if 1 //works
#define SETTINGS_SUBS_PER_ROW 8
#define SETTINGS_SUBS_PER_COL 8
#define SETTINGS_NUM_OF_SUBS  64
#define SETTINGS_NUM_OF_AUDIO_SUBS 16
#define SETTINGS_NUM_OF_CHANNELS 16
#endif

#if 0 //for testing
#define SETTINGS_SUBS_PER_ROW 2
#define SETTINGS_SUBS_PER_COL 2
#define SETTINGS_NUM_OF_SUBS  4
#define SETTINGS_NUM_OF_AUDIO_SUBS 4
#define SETTINGS_NUM_OF_CHANNELS 1
#endif




class Settings
{
    public:
        static Settings &Instance()
        {
            static Settings instance;
            return instance;
        }

        //int getNumberOfSubchannels(void);
        int getSubchannelId(int pos);
        int getSubchannelPos(int id);

        int getChannelId(int pos);
        int getChannelPos(int id);


        QList<QString> getFavouriteMidiDeviceList(void);

        QString getConnectedMidiDeviceName(void);
        void    setConnectedMidiDeviceName(QString name);
        QString getConnectedMidiDevicePort(void);
        void    setConnectedMidiDevicePort(QString port);


        bool initWithAudio(void);
        bool initWithHwMidiIn(void);
        bool initWithHwMidiOut(void);

        QString getPageName(int key);
        int getNumberOfPages(void);


        void setScreenSize(QRect screen);
        QSize getScreenSize(void);

        bool startWithFullScreen(void);

    protected:
        Settings();
        Settings(const Settings& other)
        {
            Q_UNUSED(other);
        }
        ~Settings();

    private:
        /** sub/channels **/
        QMap<int, int> *m_subchannelMap;
        void loadSubchannelMap(void);

        QMap<int, int> *m_channelMap;
        void loadChannelMap(void);

        QMap<int, QString> *m_pageMap;
        QSize m_screenSize;

        bool m_startWithFullScreen;

        /** midi **/
        QList <QString> m_midiFavList;
        QString m_connectedMidiDeviceName;
        QString m_connectedMidiDevicePort;

        int  m_numberOfSubchannels;
        void loadPageIdMap(void);
};

inline Settings& settings() { return Settings::Instance(); }

#endif // SETTINGS_H
