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

    qDebug() <<Q_FUNC_INFO <<"Settings initialized.";
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
    switch (SETTINGS_NUM_OF_SUBS)
    {
        case 64:
            // id, pos
            m_subchannelMap->insert(0,  0);
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
            break;

        case 36:
            // id, pos
            m_subchannelMap->insert(0,  0);
            m_subchannelMap->insert(1,  1);
            m_subchannelMap->insert(2,  2);
            m_subchannelMap->insert(3,  3);
            m_subchannelMap->insert(4,  4);
            m_subchannelMap->insert(5,  5);
            m_subchannelMap->insert(6,  6);
            m_subchannelMap->insert(7,  7);
            m_subchannelMap->insert(8,  8);
            m_subchannelMap->insert(9,  9);
            m_subchannelMap->insert(10,10);
            m_subchannelMap->insert(11,11);
            m_subchannelMap->insert(12,12);
            m_subchannelMap->insert(13,13);
            m_subchannelMap->insert(14,14);
            m_subchannelMap->insert(15,15);
            m_subchannelMap->insert(16,16);
            m_subchannelMap->insert(17,17);
            m_subchannelMap->insert(18,18);
            m_subchannelMap->insert(19,19);
            m_subchannelMap->insert(20,20);
            m_subchannelMap->insert(21,21);
            m_subchannelMap->insert(22,22);
            m_subchannelMap->insert(23,23);
            m_subchannelMap->insert(24,24);
            m_subchannelMap->insert(25,25);
            m_subchannelMap->insert(26,26);
            m_subchannelMap->insert(27,27);
            m_subchannelMap->insert(28,28);
            m_subchannelMap->insert(29,29);
            m_subchannelMap->insert(30,30);
            m_subchannelMap->insert(31,31);
            m_subchannelMap->insert(32,32);
            m_subchannelMap->insert(33,33);
            m_subchannelMap->insert(34,34);
            m_subchannelMap->insert(35,35);

        case 16:
            // id, pos
            m_subchannelMap->insert(0,  0);
            m_subchannelMap->insert(1,  1);
            m_subchannelMap->insert(2,  2);
            m_subchannelMap->insert(3,  3);
            m_subchannelMap->insert(4,  4);
            m_subchannelMap->insert(5,  5);
            m_subchannelMap->insert(6,  6);
            m_subchannelMap->insert(7,  7);
            m_subchannelMap->insert(8,  8);
            m_subchannelMap->insert(9,  9);
            m_subchannelMap->insert(10,10);
            m_subchannelMap->insert(11,11);
            m_subchannelMap->insert(12,12);
            m_subchannelMap->insert(13,13);
            m_subchannelMap->insert(14,14);
            m_subchannelMap->insert(15,15);
            m_subchannelMap->insert(16,16);

    default:
            break;
    }


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
#if 0
    m_channelMap->insert(10,12);
    m_channelMap->insert(11,13);
    m_channelMap->insert(12,10);
    m_channelMap->insert(13,11);
    m_channelMap->insert(14,14);
    m_channelMap->insert(15,15);
#endif
}




int Settings::getSubchannelId(int pos)
{
    qDebug() <<Q_FUNC_INFO <<"Should be obsolete. -> Function has no effect";
    return m_subchannelMap->value(pos);
}

int Settings::getSubchannelPos(int id)
{
    qDebug() <<Q_FUNC_INFO <<"Should be obsolete.";
    return m_subchannelMap->key(id);
}

int Settings::getChannelId(int pos)
{
    qDebug() <<Q_FUNC_INFO <<"Should be obsolete.";
    return m_channelMap->value(pos);
}

int Settings::getChannelPos(int id)
{
    qDebug() <<Q_FUNC_INFO <<"Should be obsolete.";
    return m_channelMap->key(id);
}





/** 2016-02-05 sub/channel get functions **/
int Settings::getNumberOfSubchannelsTotal()
{
    return SETTINGS_NUM_OF_SUBS;
}

int Settings::getNumberOfSubchannelsPerRow()
{
    return SETTINGS_SUBS_PER_ROW;
}

int Settings::getNumberOfSubchannelsPerCollum()
{
    return SETTINGS_SUBS_PER_COL;
}

int Settings::getNumberOfChannelsTotal()
{
    return SETTINGS_NUM_OF_CHANNELS;
}

int Settings::getNumberOfChannelsPerRow()
{
    return (SETTINGS_SUBS_PER_ROW / 2);
}

int Settings::getNumberOfChannelsPerCollum()
{
    return (SETTINGS_SUBS_PER_COL / 2);
}



int Settings::getChannelOfSubchannel(int sub)
{
    int channel;

#if SETTINGS_NUM_OF_SUBS == 36
    switch (sub)
    {
        case 0:
        case 1:
        case 6:
        case 7:
        channel = 0;
        break;

        case 2:
        case 3:
        case 8:
        case 9:
        channel = 1;
        break;

        case 4:
        case 5:
        case 10:
        case 11:
        channel = 2;
        break;

        case 12:
        case 13:
        case 18:
        case 19:
        channel = 3;
        break;

        case 14:
        case 15:
        case 20:
        case 21:
        channel = 4;
        break;

        case 16:
        case 17:
        case 22:
        case 23:
        channel = 5;
        break;

        case 24:
        case 25:
        case 30:
        case 31:
        channel = 6;
        break;

        case 26:
        case 27:
        case 32:
        case 33:
        channel = 7;
        break;

        case 28:
        case 29:
        case 34:
        case 35:
        channel = 8;
        break;

        default:
        break;
    }
#endif

#if SETTINGS_NUM_OF_SUBS == 16
    switch (sub)
    {
        case 0:
        case 1:
        case 4:
        case 5:
        channel = 0;
        break;

        case 2:
        case 3:
        case 6:
        case 7:
        channel = 1;
        break;

        case 8:
        case 9:
        case 12:
        case 13:
        channel = 2;
        break;

        case 10:
        case 11:
        case 14:
        case 15:
        channel = 3;
        break;

        default:
        break;
    }
#endif

    return channel;
}



int Settings::getFirstSubchannelOfChannel(int ch)
{
    int firstSubchannel;

#if SETTINGS_NUM_OF_SUBS == 36
    switch (ch)
    {
        case 0: firstSubchannel = 0;  break;
        case 1: firstSubchannel = 2;  break;
        case 2: firstSubchannel = 4;  break;
        case 3: firstSubchannel = 12; break;
        case 4: firstSubchannel = 14; break;
        case 5: firstSubchannel = 16; break;
        case 6: firstSubchannel = 24; break;
        case 7: firstSubchannel = 26; break;
        case 8: firstSubchannel = 28; break;
        default: break;
    }
#endif

#if SETTINGS_NUM_OF_SUBS == 16
    switch (ch)
    {
        case 0: firstSubchannel = 0;  break;
        case 1: firstSubchannel = 2;  break;
        case 2: firstSubchannel = 8;  break;
        case 3: firstSubchannel = 10; break;
        default: break;
    }
#endif

    return firstSubchannel;
}



QPoint Settings::getCoordinatesOfSubchannel(int sub)
{
    QPoint coordinates;
    coordinates.setX( sub % SETTINGS_NUM_OF_SUBS);
    coordinates.setY( sub / SETTINGS_NUM_OF_SUBS);
    return coordinates;
}



QList<int> Settings::getSubchannelsOfChannel(int ch)
{
    QList<int> subchannels;

#if SETTINGS_NUM_OF_SUBS == 36
    switch (ch)
    {
        case 0:
            subchannels.append(0);
            subchannels.append(1);
            subchannels.append(6);
            subchannels.append(7);
            break;
        case 1:
            subchannels.append(2);
            subchannels.append(3);
            subchannels.append(8);
            subchannels.append(9);
            break;
        case 2:
            subchannels.append(4);
            subchannels.append(5);
            subchannels.append(10);
            subchannels.append(11);
            break;
        case 3:
            subchannels.append(12);
            subchannels.append(13);
            subchannels.append(18);
            subchannels.append(19);
            break;
        case 4:
            subchannels.append(14);
            subchannels.append(15);
            subchannels.append(20);
            subchannels.append(21);
            break;
        case 5:
            subchannels.append(16);
            subchannels.append(17);
            subchannels.append(22);
            subchannels.append(23);
            break;
        case 6:
            subchannels.append(24);
            subchannels.append(25);
            subchannels.append(30);
            subchannels.append(31);
            break;
        case 7:
            subchannels.append(26);
            subchannels.append(27);
            subchannels.append(32);
            subchannels.append(33);
            break;
        case 8:
            subchannels.append(28);
            subchannels.append(29);
            subchannels.append(34);
            subchannels.append(35);
            break;
        default:
            break;
    }
#endif

#if SETTINGS_NUM_OF_SUBS == 16
    switch (ch)
    {
        case 0:
            subchannels.append(0);
            subchannels.append(1);
            subchannels.append(4);
            subchannels.append(5);
            break;
        case 1:
            subchannels.append(2);
            subchannels.append(3);
            subchannels.append(6);
            subchannels.append(7);
            break;
        case 2:
            subchannels.append(8);
            subchannels.append(9);
            subchannels.append(12);
            subchannels.append(13);
            break;
        case 3:
            subchannels.append(10);
            subchannels.append(11);
            subchannels.append(14);
            subchannels.append(15);
            break;
    }
#endif

    return subchannels;
}



int Settings::getRelativeSubchannel(int sub)
{
    int relSub;

#if SETTINGS_NUM_OF_SUBS == 36
    switch (sub)
    {
        case 0:
        case 2:
        case 4:
        case 12:
        case 14:
        case 16:
        case 24:
        case 26:
        case 28:
        relSub = 0;
        break;

        case 1:
        case 3:
        case 5:
        case 13:
        case 15:
        case 17:
        case 25:
        case 27:
        case 29:
        relSub = 1;
        break;

        case 6:
        case 8:
        case 10:
        case 18:
        case 20:
        case 22:
        case 30:
        case 32:
        case 34:
        relSub = 2;
        break;

        case 7:
        case 9:
        case 11:
        case 19:
        case 21:
        case 23:
        case 31:
        case 33:
        case 35:
        relSub = 3;
        break;

        default:
            break;
    }
#endif


#if SETTINGS_NUM_OF_SUBS == 16
    switch (sub)
    {
        case 0:
        case 2:
        case 8:
        case 10:
        relSub = 0;
        break;

        case 1:
        case 3:
        case 9:
        case 11:
        relSub = 1;
        break;

        case 4:
        case 6:
        case 12:
        case 14:
        relSub = 2;
        break;

        case 5:
        case 7:
        case 13:
        case 15:
        relSub = 3;
        break;

        default:
        break;
    }
#endif

    return relSub;
}







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



