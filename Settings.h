#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSize>
#include <QList>


/** ****************************************************************************
    USER SETTINGS
*******************************************************************************/

#define SETTINGS_INIT_WITH_AUDIO        false
#define SETTINGS_START_WITH_FULL_SCREEN false
#define SETTINGS_SUBS_PER_ROW           6

/** ***************************************************************************/



// 2016-02-26 should be obsolete
#define SETTINGS_INIT_WITH_MIDI_IN      false
#define SETTINGS_INIT_WITH_MIDI_OUT     false

// 2016-02-26 subchannel tile calculations
#define SETTINGS_SUBS_PER_COL SETTINGS_SUBS_PER_ROW
#define SETTINGS_NUM_OF_SUBS (SETTINGS_SUBS_PER_ROW * SETTINGS_SUBS_PER_COL)
#define SETTINGS_NUM_OF_CHANNELS (SETTINGS_NUM_OF_SUBS/4)

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

#if 1 //
        int getNumberOfSubchannelsTotal(void);
        int getNumberOfSubchannelsPerRow(void);
        int getNumberOfSubchannelsPerCollum(void);
        int getNumberOfChannelsTotal(void);
        int getNumberOfChannelsPerRow(void);
        int getNumberOfChannelsPerCollum(void);

        int getChannelOfSubchannel(int sub);
        int getFirstSubchannelOfChannel(int ch);
        QPoint getCoordinatesOfSubchannel(int sub);
        QList<int> getSubchannelsOfChannel(int ch);
        int getRelativeSubchannel(int sub);
#endif

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
        void loadSubchannelMapVar(void);

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
