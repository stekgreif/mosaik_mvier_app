#include "ui/UiManager.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include <core/subchannel/SubchannelManager.h>
#include "Settings.h"

#include "MosaikMiniApp.h"

#define POS_OFFSET 550


UiManager::UiManager(MosaikMiniApp *mosaikMiniApp, QWidget *uiParent)
    : QWidget(uiParent)
    , m_mosaikMiniApp(mosaikMiniApp)
{
    qDebug() <<Q_FUNC_INFO <<"Init.";

    this->setObjectName("uiManager");


	/** size of actual screen **/
	int screenHeight = settings().getScreenSize().height();
	int screenWidth = settings().getScreenSize().width();

    /** pages **/
    m_pageSubchannel = new PageSubchannel(m_mosaikMiniApp, this);
    m_pageInfo = new PageInfos(m_mosaikMiniApp);
    m_pageTest = new PageTest(m_mosaikMiniApp);

    m_pageSelection = new PageSelection(this);
	m_pageSelection->setFixedSize(screenHeight,screenHeight);
    m_pageSelection->move(0,0);


    /** page stack main element - position of right ui side **/
    m_stackedPages = new QTabWidget(this);
    m_stackedPages->setObjectName("pageStack");
	m_stackedPages->setFixedSize(screenWidth-screenHeight-10, screenHeight);
	m_stackedPages->move(screenHeight+4, 0);
	m_stackedPages->setTabPosition(QTabWidget::South);

	m_stackedPages->addTab(m_pageSubchannel, "      Sub      ");
	m_stackedPages->addTab(m_pageInfo, "      Info      ");
	m_stackedPages->addTab(m_pageTest, "      Test      ");

    m_stackedPages->setCurrentIndex(0);

	const int interval = 6;
	const int offset = 77;

	/** bottom labels **/
	m_labelPreVol = new QLabel("pre vol", this);
	m_labelPreVol->setObjectName("headLabel");
	m_labelPreVol->move( (offset + interval*0) * screenWidth/100, 98*screenHeight/100 );

	m_labelMainVol = new QLabel("main vol", this);
	m_labelMainVol->setObjectName("headLabel");
	m_labelMainVol->move( (offset + interval*1) * screenWidth/100, 98*screenHeight/100 );

	m_labelBpm = new QLabel(this);
	m_labelBpm->setObjectName("bpmLabel");
	m_labelBpm->move( (offset + interval*2) * screenWidth/100, 98*screenHeight/100 );
	m_labelBpm->setText( QString::number( subchannelManager().getBpm(), 'f', 2 ) + "  bpm" );

	m_labelStepCounter = new QLabel(this);
    m_labelStepCounter->setObjectName("stepcounter");
	m_labelStepCounter->move( (offset + interval*3) * screenWidth/100, 98*screenHeight/100 );
    m_labelStepCounter->setText("NOT PLAYING");

	refreshMainVol();
    refreshPreVol();


    /** signal slot connections **/
    connect(m_pageSelection,  SIGNAL(signal_subchannelSelectionPadPressed(int)), this, SLOT(slot_subchannelSelectionPadTriggert(int)));

#if 0
    m_timer = new QTimer;
    m_timer->start(100);
    connect( m_timer, SIGNAL(timeout()), this,  SLOT(slot_regularTimer()) );
#endif
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
	m_labelBpm->setText( QString::number(subchannelManager().getBpm(), 'f', 2 ) + "bpm" );
    m_pageSubchannel->refreshLabels();
    m_pageSubchannel->refreshStepAxis();
}

void UiManager::refreshPreVol()
{
	m_labelPreVol->setText( "HP: " + QString::number( subchannelManager().getPreVolume(), 'f', 2 ) );
}

void UiManager::refreshMainVol()
{
	m_labelMainVol->setText( "PA: " + QString::number( subchannelManager().getMainVolume(), 'f', 2 ) );
}

void UiManager::refreshMutePads()
{
    m_pageSelection->refreshMutePads();
}

void UiManager::refreshSelectionPad()
{
    m_pageSelection->refreshSampleVolume();
}

void UiManager::refreshSampleVolumeWidget(int id)
{
    m_pageSelection->refreshSampleVolumeWidget(id);
}

void UiManager::refreshPlayDirection()
{
    m_pageSubchannel->slot_refreshPlayDirection();
    m_pageSelection->refreshPlayDirection();
}

void UiManager::slot_browserOpenFolder()
{
    m_pageSubchannel->slot_browserOpenFolder();
}

void UiManager::slot_browserCloseFolder()
{
    m_pageSubchannel->slot_browserCloseFolder();
}



void UiManager::slot_toggleMuteAndSolo()
{
    //qDebug() <<Q_FUNC_INFO;
    m_pageSelection->toggleMuteAndSolo();
}


/** browser **/
void UiManager::loadSelectedSampleToCurrentSubchannel()
{
    m_pageSubchannel->slot_loadSample();
    m_pageSelection->refresh();
    m_pageSelection->refreshWaveFormWindow();
}

void UiManager::loadSelectedSampleToPrelisten()
{
    m_pageSubchannel->slot_loadSampleToPrelisten();
    m_pageSelection->refresh();
}

void UiManager::slot_setPathId(int pathId)
{
    m_pageSubchannel->slot_changePath(pathId);
}



int UiManager::getCurrentPageId()
{
    return m_stackedPages->currentIndex();
}

MosaikMiniApp *UiManager::getParent()
{
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

