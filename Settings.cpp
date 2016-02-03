#include "Settings.h"
#include <QMap>
#include <QRect>
#include <QDebug>
#include <QString>
#include <QList>




Settings::Settings()
    : m_subchannelMap(new QMap<int, int>)
    , m_channelMap(new QMap<int, int>)
    , m_pageMap(new QMap<int, QString>)
    //, m_midiFavList(new QList<QString>)
{
    loadSubchannelMap();
    loadChannelMap();
    loadPageIdMap();
    m_numberOfSubchannels = SETTINGS_NUM_OF_SUBS;
    m_startWithFullScreen = SETTINGS_START_WITH_FULL_SCREEN;

    m_connectedMidiDeviceName.clear();
    m_connectedMidiDevicePort.clear();

    m_midiFavList.append("MOSAIKmini");
    m_midiFavList.append("SparkLE");
    m_midiFavList.append("Mosaik");
    m_midiFavList.append("HELLA-UNTZtrument");
}

Settings::~Settings()
{
    qDebug() <<"~" <<Q_FUNC_INFO;
}

bool Settings::startWithFullScreen(void)
{
    return m_startWithFullScreen;
}



void Settings::loadPageIdMap(void)
{
    m_pageMap->insert(0, "Selection");
    m_pageMap->insert(1, "Youtube");
    m_pageMap->insert(2, "Debug");
    m_pageMap->insert(3, "Settings");
    m_pageMap->insert(4, "Info");
    //m_pageMap->insert(0, "Subchannel");
}

QString Settings::getPageName(int key)
{
    return m_pageMap->value(key);
}

int Settings::getNumberOfPages(void)
{
    return m_pageMap->size();
}

QString Settings::getConnectedMidiDeviceName()
{
    return m_connectedMidiDeviceName;
}

void Settings::setConnectedMidiDeviceName(QString name)
{
    m_connectedMidiDeviceName = name;
}

QString Settings::getConnectedMidiDevicePort()
{
    return m_connectedMidiDevicePort;
}

void Settings::setConnectedMidiDevicePort(QString port)
{
    m_connectedMidiDevicePort = port;
}

void Settings::setScreenSize(QRect screen)
{
    m_screenSize.setWidth(screen.width());
    m_screenSize.setHeight(screen.height());

    qDebug() <<Q_FUNC_INFO <<"screensize:" <<m_screenSize;
}

QSize Settings::getScreenSize()
{
    return m_screenSize;
}






void Settings::loadSubchannelMap(void)
{
    m_subchannelMap->insert(0,  0); // id, pos
    m_subchannelMap->insert(1,  1);
    m_subchannelMap->insert(2,  4);
    m_subchannelMap->insert(3,  5);
    m_subchannelMap->insert(4, 16);
    m_subchannelMap->insert(5, 17);
    m_subchannelMap->insert(6, 20);
    m_subchannelMap->insert(7, 21);

    m_subchannelMap->insert(8,  2);
    m_subchannelMap->insert(9,  3);
    m_subchannelMap->insert(10, 6);
    m_subchannelMap->insert(11, 7);
    m_subchannelMap->insert(12,18);
    m_subchannelMap->insert(13,19);
    m_subchannelMap->insert(14,22);
    m_subchannelMap->insert(15,23);

    m_subchannelMap->insert(16, 8);
    m_subchannelMap->insert(17, 9);
    m_subchannelMap->insert(18,12);
    m_subchannelMap->insert(19,13);
    m_subchannelMap->insert(20,24);
    m_subchannelMap->insert(21,25);
    m_subchannelMap->insert(22,28);
    m_subchannelMap->insert(23,29);

    m_subchannelMap->insert(24,10);
    m_subchannelMap->insert(25,11);
    m_subchannelMap->insert(26,14);
    m_subchannelMap->insert(27,15);
    m_subchannelMap->insert(28,26);
    m_subchannelMap->insert(29,27);
    m_subchannelMap->insert(30,30);
    m_subchannelMap->insert(31,31);

    m_subchannelMap->insert(32,32);
    m_subchannelMap->insert(33,33);
    m_subchannelMap->insert(34,36);
    m_subchannelMap->insert(35,37);
    m_subchannelMap->insert(36,48);
    m_subchannelMap->insert(37,49);
    m_subchannelMap->insert(38,52);
    m_subchannelMap->insert(39,53);

    m_subchannelMap->insert(40,34);
    m_subchannelMap->insert(41,35);
    m_subchannelMap->insert(42,38);
    m_subchannelMap->insert(43,39);
    m_subchannelMap->insert(44,50);
    m_subchannelMap->insert(45,51);
    m_subchannelMap->insert(46,54);
    m_subchannelMap->insert(47,55);

    m_subchannelMap->insert(48,40);
    m_subchannelMap->insert(49,41);
    m_subchannelMap->insert(50,44);
    m_subchannelMap->insert(51,45);
    m_subchannelMap->insert(52,56);
    m_subchannelMap->insert(53,57);
    m_subchannelMap->insert(54,60);
    m_subchannelMap->insert(55,61);

    m_subchannelMap->insert(56,42);
    m_subchannelMap->insert(57,43);
    m_subchannelMap->insert(58,46);
    m_subchannelMap->insert(59,47);
    m_subchannelMap->insert(60,58);
    m_subchannelMap->insert(61,59);
    m_subchannelMap->insert(62,62);
    m_subchannelMap->insert(63,63);
}



/*
    00 01 04 05
    02 03 06 07
    08 09 12 13
    10 11 14 15
*/
void Settings::loadChannelMap()
{
    m_channelMap->insert( 0, 0); // id, pos
    m_channelMap->insert( 1, 1);
    m_channelMap->insert( 2, 4);
    m_channelMap->insert( 3, 5);
    m_channelMap->insert( 4, 2);
    m_channelMap->insert( 5, 3);
    m_channelMap->insert( 6, 6);
    m_channelMap->insert( 7, 7);
    m_channelMap->insert( 8, 8);
    m_channelMap->insert( 9, 9);
    m_channelMap->insert(10,12);
    m_channelMap->insert(11,13);
    m_channelMap->insert(12,10);
    m_channelMap->insert(13,11);
    m_channelMap->insert(14,14);
    m_channelMap->insert(15,15);
}


/* positions:
    00 01 02 03 04 05 06 07
    08 09 .. .. .. .. .. ..
    .. .. .. .. .. .. .. ..
    .. .. .. .. .. .. .. ..
    .. .. .. .. .. .. .. ..
    .. .. .. .. .. .. .. ..
    .. .. .. .. .. .. .. ..
    56 57 58 59 60 61 62 63  */
int Settings::getSubchannelId(int pos)
{
    return m_subchannelMap->value(pos);
}

int Settings::getSubchannelPos(int id)
{
    return m_subchannelMap->key(id);
}

int Settings::getChannelId(int pos)
{
    return m_channelMap->value(pos);
}

int Settings::getChannelPos(int id)
{
    return m_channelMap->key(id);
}









#if 0 // should be obsolete
int Settings::getNumberOfSubchannels(void)
{
    return m_numberOfSubchannels;
}
#endif

QList<QString> Settings::getFavouriteMidiDeviceList()
{
    return m_midiFavList;
}

bool Settings::initWithAudio()
{
    return SETTINGS_INIT_WITH_AUDIO;
}

bool Settings::initWithHwMidiIn()
{
    return SETTINGS_INIT_WITH_MIDI_IN;
}

bool Settings::initWithHwMidiOut()
{
    return SETTINGS_INIT_WITH_MIDI_OUT;
}



