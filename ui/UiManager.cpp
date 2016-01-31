#include "UiManager.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include <ui/sidepanels/SubchannelPreview.h>
#include <core/subchannel/SubchannelManager.h>
#include "Settings.h"

#include "MosaikMiniApp.h"



UiManager::UiManager(MosaikMiniApp *mosaikMiniApp, QWidget *uiParent)
    : QWidget(uiParent)
    , m_mosaikMiniApp(mosaikMiniApp)
{
    qDebug() <<Q_FUNC_INFO <<"Init.";

    this->setObjectName("uiManager");

    int h = settings().getScreenSize().height();
    int w = settings().getScreenSize().width();

    /** pages **/
    m_pageSubchannel = new PageSubchannel(m_mosaikMiniApp, this);
    //m_pageBrowser    = new PageBrowser(this);
    m_pageSelection = new PageSelection(this);
    m_pageSelection->setFixedSize(h,h);
    m_pageSelection->move(0,0);


    /** page stack main element **/
    m_stackedPages = new QStackedWidget(this);
    m_stackedPages->setObjectName("pageStack");
    m_stackedPages->setFixedSize(w-h-20 , h-20);
    m_stackedPages->move(h+20, 20);

    m_stackedPages->addWidget(m_pageSubchannel);
    //m_stackedPages->addWidget(m_pageBrowser);
    m_stackedPages->setCurrentIndex(0);


    /** right side elements **/
    m_labelStepCounter = new QLabel(this);
    m_labelStepCounter->setObjectName("stepcounter");
    m_labelStepCounter->move(1800,2);
    m_labelStepCounter->setText("NOT PLAYING");


    /** head labels **/
    m_labelBpm = new QLabel(this);
    m_labelBpm->setObjectName("bpmLabel");
    m_labelBpm->move(1700, 2);
    m_labelBpm->setText( QString::number( subchannelManager().getBpm(), 'f', 2 ) + "  bpm" );


    m_labelMainVol = new QLabel("main vol", this);
    m_labelMainVol->setObjectName("headLabel");
    m_labelMainVol->move(1450, 2);
    refreshMainVol();

    m_labelPreVol = new QLabel("pre vol", this);
    m_labelPreVol->setObjectName("headLabel");
    m_labelPreVol->move(1590, 2);
    refreshPreVol();


    /** signal slot connections **/
    connect(m_pageSelection,  SIGNAL(signal_subchannelSelectionPadPressed(int)), this, SLOT(slot_subchannelSelectionPadTriggert(int)));
    connect(m_pageSubchannel, SIGNAL(signal_btnLoadSamplePressed()), this, SLOT(refresh()) );

    m_timer = new QTimer;
    m_timer->start(100);
    connect( m_timer, SIGNAL(timeout()), this,  SLOT(slot_regularTimer()) );
}



void UiManager::slot_regularTimer()
{
    //qDebug() <<Q_FUNC_INFO <<"";
    m_pageSelection->refreshTiles();
}




void UiManager::slot_subchannelSelectionPadTriggert(int id)
{
    //qDebug() <<Q_FUNC_INFO <<id;
    emit signal_subchannelSelectionPadTriggert(id);
}


UiManager::~UiManager()
{
    delete m_labelStepCounter;
    delete m_stackedPages;
    delete m_pageSelection;
    //delete m_pageSubchannel;

    qDebug() <<"~" <<Q_FUNC_INFO;
}


void UiManager::setPageIndex(int id)
{
    m_stackedPages->setCurrentIndex(id);
    refresh();
}



void UiManager::refresh()
{
    qDebug() <<Q_FUNC_INFO;

    m_pageSelection->refresh();

    switch (m_stackedPages->currentIndex())
    {
        case 0:
            m_pageSubchannel->refresh();
            break;
        case 1:
            //sample browser
            break;
        case 2:
            // page mute/solo
            break;
        case 3:
            m_pageSettings->refresh();
            break;
        case 4:
            m_pageInfos->refresh();
            break;
        case 5:
            m_pageDebug->refresh();
            break;
        default:
            break;
    }
}

void UiManager::refreshSelection()
{
    m_pageSelection->refresh();
}

void UiManager::refreshEnvelope()
{
    qDebug() <<Q_FUNC_INFO;
    m_pageSubchannel->refreshEnvelope();
}

void UiManager::refreshStepCounterAbsolute(quint64 cnt)
{
    m_labelStepCounter->setText(QString::number(cnt%64));
    m_labelStepCounter->adjustSize();
}

void UiManager::refreshVolPoti()
{
    m_pageSubchannel->refreshLabels();
}

void UiManager::refreshBpm()
{
    m_labelBpm->setText( QString::number( subchannelManager().getBpm(), 'f', 2 ) + "  bpm" );
    m_pageSubchannel->refreshLabels();
    m_pageSubchannel->refreshStepAxis();
}

void UiManager::refreshPreVol()
{
    m_labelPreVol->setText( "Pre Vol: " + QString::number( subchannelManager().getPreVolume(), 'f', 2 ) );
}

void UiManager::refreshMainVol()
{
    m_labelMainVol->setText( "Main Vol: " + QString::number( subchannelManager().getMainVolume(), 'f', 2 ) );
}

void UiManager::refreshMutePads()
{
    m_pageSelection->refreshMutePads();
}

void UiManager::refreshSelectionPad()
{
    m_pageSelection->refreshSampleVolume();
}

void UiManager::slot_toggleMuteAndSolo()
{
    qDebug() <<Q_FUNC_INFO;
    m_pageSelection->toggleMuteAndSolo();
}

void UiManager::loadSelectedSampleToCurrentSubchannel()
{
    //m_pageBrowser->slot_loadSample();
}

void UiManager::loadSelectedSampleToPrelisten()
{
    //m_pageBrowser->slot_loadSampleToPrelisten();
}

void UiManager::slot_setPathId(int pathId)
{
    //m_pageBrowser->slot_changePathId(pathId);
    //m_pageSubchannel->slot_changePathId(pathId);
}



int UiManager::getCurrentPageId()
{
    return m_stackedPages->currentIndex();
}

MosaikMiniApp *UiManager::getParent()
{
    //return m_parent;
    return m_mosaikMiniApp;
}




void UiManager::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption myOption;
    myOption.initFrom(this);
    QPainter myPainter(this);
    style()->drawPrimitive( QStyle::PE_Widget, &myOption, &myPainter,this);
}

