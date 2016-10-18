#include "MosaikMiniApp.h"
#include "ui_MosaikMiniApp.h"

#include <QtWidgets>
#include <QScrollArea>
#include <QDesktopWidget>
#include <QDebug>

#include "Settings.h"
#include "MosaikTypes.h"

#include "ui/pages/subchannel/PageSubchannel.h"

#include "core/audio/AlsaPcm.h"
#include "core/midi/MidiInfo.h"
#include "core/midi/MidiManager.h"
#include "core/midi/MidiNames.h"



MosaikMiniApp::MosaikMiniApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MosaikMiniApp)
    , m_scrollArea(new QScrollArea(this))
    , m_screenInfo(new QDesktopWidget)
{
    /** CORE **/
    subchannelManager().init();
    settings().setScreenSize(m_screenInfo->screenGeometry());

    /** UI **/
    this->setCursor(Qt::CrossCursor);
    //this->setCursor(Qt::BlankCursor);

    ui->setupUi(this);
    ui->statusBar->hide();
    ui->mainToolBar->hide();
    ui->menuBar->hide();
    //this->setContentsMargins(1,1,1,1);


    m_uiManager = new UiManager(this, m_scrollArea);
    m_uiManager->setFixedSize( settings().getScreenSize() );

    m_scrollArea->setFrameStyle(QFrame::NoFrame);
    m_scrollArea->setWidget(m_uiManager);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setCentralWidget(m_scrollArea);


    /** general **/
    if( settings().startWithFullScreen() )
    {
        slot_setFullScreen();
    }
    else
    {
        slot_setNormalScreen();
    }


    /** MIDI **/
    midiManager().setParentWidget(this);
    midiManager().connectFavouriteDevice();



    if( settings().initWithAudio() )
    {
        m_alsaPcm = new AlsaPcm;

        float randomBpm = rand() % 110 + 70;
        subchannelManager().setBpm(randomBpm);
        m_alsaPcm->slot_bpmChanged(subchannelManager().getBpm());
        m_uiManager->refreshBpm();

        m_alsaPcm->start();
    }

    m_stepCounter = -1;
    m_timer = new QTimer;
    m_timer->start(10);


    /** signal slot connections **/
    connect( m_uiManager,    SIGNAL(signal_subchannelSelectionPadTriggert(int)), this, SLOT(slot_subchannelSelectionPadTriggert(int)));
    connect( m_timer, SIGNAL(timeout()), this,  SLOT(slot_regularTimer()) );

}

MosaikMiniApp::~MosaikMiniApp()
{
    midiManager().resetHardware();
    delete m_timer;
    if( settings().initWithAudio() )
    {
        m_alsaPcm->stop();
        delete m_alsaPcm;
    }
    delete m_uiManager;
    delete m_screenInfo;
    delete m_scrollArea;
    delete ui;
}


void MosaikMiniApp::slot_seqButtonChanged(quint8 id, quint8 val)
{
    if (id < 64)
    {
        if(val == 127)
        {
            subchannelManager().toggleStep(id);
        }
        m_uiManager->refresh();
    }
    else
    {
        qDebug() <<Q_FUNC_INFO <<"Step id" <<id <<"not in range";
    }
}


void MosaikMiniApp::slot_fnlButtonChanged(quint8 id, quint8 val)
{
    if ((id < settings().getNumberOfPages()) && (val == 127))
    {
        m_uiManager->setPageIndex(id);
        //qDebug() <<Q_FUNC_INFO <<"Page changed";
    }
}


void MosaikMiniApp::slot_fnrButtonChanged(quint8 id, quint8 val)
{
    Q_UNUSED(val);

    qDebug() <<Q_FUNC_INFO;
    switch(id)
    {
        case 1:
            slot_exitApplication();
            break;
        case 2:
            slot_setFullScreen();
            break;
        case 3:
            slot_setNormalScreen();
            break;
        case 15:
            subchannelManager().clearCurrentSubchannelPattern();
            m_uiManager->refresh();
            //m_midiOutRenderer->rstSeqLeds();
        default:
            qDebug() <<"default";
            break;
    }
}


#if 1
void MosaikMiniApp::slot_sparkEvent(quint8 id, int val)
{
    qDebug() <<Q_FUNC_INFO <<"midi in val:" <<val;
    float fVal = (float)val / 100;
    qDebug() <<Q_FUNC_INFO <<"relative val:" <<val;

    switch (id)
    {
        case 48: //start
            subchannelManager().setCurrentStartPointRel(fVal);
            break;
        case 52: //fade in
            subchannelManager().setCurrentFadeInPointRel(fVal);
            break;
        case 53: //fade out
            subchannelManager().setCurrentFadeOutPointRel(fVal);
            break;
        case 47: //end
            subchannelManager().setCurrentEndPointRel(fVal);
            break;
        default:
            break;
    }

    m_uiManager->refresh();
}
#endif



void MosaikMiniApp::slot_stepButtonPressed(int id)
{
    qDebug() <<Q_FUNC_INFO <<id;
    subchannelManager().toggleStep(id);
    midiManager().refreshPatternView();
    m_uiManager->refresh();
}



void MosaikMiniApp::slot_regularTimer()
{
    if(settings().initWithAudio())
    {
        quint32 stepCounter = m_alsaPcm->getStepCounter();

        if( m_stepCounter != stepCounter )
        {
            m_stepCounter = stepCounter;
            //qDebug() <<Q_FUNC_INFO <<"Update Step Counter:" <<m_stepCounter;
            m_uiManager->refreshStepCounterAbsolute( m_stepCounter );
            midiManager().setStepsequencerLed(m_stepCounter % 64);
        }
    }
}



/** ****************************************************************************
    sample
*******************************************************************************/
void MosaikMiniApp::slot_sampleUnloadCurrentSubchannel()
{
    subchannelManager().unloadSampleOfCurrentSubchannel();
    m_uiManager->refresh();
}

void MosaikMiniApp::slot_sampleUnloadCurrentChannel()
{
    subchannelManager().unloadSamplesOfCurrentChannel();
    m_uiManager->refresh();
}

void MosaikMiniApp::slot_sampleUnloadAll()
{
    subchannelManager().unloadAllSamples();
    m_uiManager->refresh();
}

void MosaikMiniApp::slot_sampleLoadToCurrentSubchannel()
{
    qDebug() <<Q_FUNC_INFO;
    m_uiManager->loadSelectedSampleToCurrentSubchannel();
    m_uiManager->refresh();
}

void MosaikMiniApp::slot_sampleLoadToPrelisten()
{
    qDebug() <<Q_FUNC_INFO;
    m_uiManager->loadSelectedSampleToPrelisten();
}

void MosaikMiniApp::slot_prelistenSubchannelSample()
{
    qDebug() <<Q_FUNC_INFO;
    subchannelManager().prelistenCurrentSubchannelSample();
}




/** ****************************************************************************
    selection
*******************************************************************************/
void MosaikMiniApp::slot_selectionSetCurrentSubchannelRelative(int id)
{
    subchannelManager().setCurrentSubchannelSelectionRelative(id);
    midiManager().refreshPatternView();
    midiManager().refreshSubchannelSelection();
    m_uiManager->refresh();
}

void MosaikMiniApp::slot_selectionSetCurrentChannelRelative(int relVal)
{
    qDebug() <<Q_FUNC_INFO <<"relVal" <<relVal;
    subchannelManager().setCurrentChannelRelative(relVal);
    m_uiManager->refresh();
}

void MosaikMiniApp::slot_selectionSetCurrentChannel(int id)
{
    qDebug() <<Q_FUNC_INFO <<"id" <<id;
    subchannelManager().setCurrentChannelSelection(id);
    midiManager().refreshSubchannelSelection();
    m_uiManager->refresh();
}

void MosaikMiniApp::slot_selectionNextSubchannel()
{
    qDebug() <<Q_FUNC_INFO;
    subchannelManager().selectNextSubChannel();
    midiManager().refreshPatternView();
    midiManager().refreshSubchannelSelection();
    m_uiManager->refresh();
}



/** ****************************************************************************
    pattern
*******************************************************************************/
void MosaikMiniApp::slot_patternClearCurrentSubchannel()
{
    subchannelManager().clearCurrentSubchannelPattern();
    midiManager().refreshPatternView();
    m_uiManager->refresh();
}

void MosaikMiniApp::slot_patternClearCurrentChannel()
{
    subchannelManager().clearCurrentChannelPattern();
    midiManager().refreshPatternView();
    m_uiManager->refresh();
}

void MosaikMiniApp::slot_patternClearAll()
{
    subchannelManager().clearAllPattern();
    midiManager().refreshPatternView();
    m_uiManager->refresh();
}



/** ****************************************************************************
    pages
*******************************************************************************/
void MosaikMiniApp::slot_uiSetToPageSubchannel()
{
    m_uiManager->setPageIndex(Mosaik::Pages::Subchannel);
}

void MosaikMiniApp::slot_uiSetToPageBrowser()
{
    m_uiManager->setPageIndex(Mosaik::Pages::Browser);
}


/** ****************************************************************************
    browser
*******************************************************************************/
void MosaikMiniApp::slot_browserChangePathId(int id)
{
    m_uiManager->slot_setPathId(id);
    m_uiManager->refresh();
}

void MosaikMiniApp::slot_browserChangeCursorPosition(int direction)
{
    qDebug() <<Q_FUNC_INFO;
    m_uiManager->m_pageSubchannel->slot_moveCursor(direction);
}

void MosaikMiniApp::slot_browserToggleFolderExpansion()
{
    m_uiManager->m_pageSubchannel->slot_toggleItmeExpansion();
}

void MosaikMiniApp::slot_browserSelectedSampleToPrelisten()
{
    qDebug() <<Q_FUNC_INFO;
    m_uiManager->loadSelectedSampleToPrelisten();
}

void MosaikMiniApp::slot_browserOpenFolder()
{
    m_uiManager->slot_browserOpenFolder();
}

void MosaikMiniApp::slot_browserCloseFolder()
{
    m_uiManager->slot_browserCloseFolder();
}



/** ****************************************************************************
    envelope and audio parameter
*******************************************************************************/

void MosaikMiniApp::slot_envelopeChangeCurrentSubchFadeIn(float relVal)
{
    subchannelManager().setCurrentFadeInPointRel(relVal);
    m_uiManager->refreshEnvelope();
}

void MosaikMiniApp::slot_envelopeChangeCurrentSubchStart(float relVal)
{
    subchannelManager().setCurrentStartPointRel(relVal);
    m_uiManager->refreshEnvelope();
}

void MosaikMiniApp::slot_envelopeChangeCurrentSubchEnd(float relVal)
{
    subchannelManager().setCurrentEndPointRel(relVal);
    m_uiManager->refreshEnvelope();
}

void MosaikMiniApp::slot_envelopeChangeCurrentSubchFadeOut(float relVal)
{
    subchannelManager().setCurrentFadeOutPointRel(relVal);
    m_uiManager->refreshEnvelope();
}

void MosaikMiniApp::slot_parameterChangeCurrentSubchVolume(float relVal)
{
    subchannelManager().setCurrentSubchannelVolumeRelative(relVal);
    m_uiManager->refreshVolPoti();
}




void MosaikMiniApp::slot_parameterPan(float relVal)
{
    subchannelManager().setCurrentPan(relVal);
    m_uiManager->refreshVolPoti();
}

void MosaikMiniApp::slot_parameterCurrentSubToPre(bool state)
{
    qDebug() <<Q_FUNC_INFO <<"state" <<state;
    subchannelManager().currentSubchannelToPrelisten(state);
    midiManager().subToPreLed(state);
}

void MosaikMiniApp::slot_parameterMuteAndSolo(bool state)
{
    //qDebug() <<Q_FUNC_INFO <<"state" <<state;
    m_uiManager->slot_toggleMuteAndSolo();
}

void MosaikMiniApp::slot_parameterUnmuteAll()
{
    subchannelManager().unmuteAll();
    m_uiManager->refreshMutePads();
}

void MosaikMiniApp::slot_parameterSelectLastMutes()
{
    subchannelManager().lastMuteStates();
    m_uiManager->refreshMutePads();
}

void MosaikMiniApp::slot_parameterPlayDirection(bool direction)
{
    qDebug() <<Q_FUNC_INFO <<direction;
    subchannelManager().setCurrentPlayDirection(direction);
    m_uiManager->refreshPlayDirection();
}


void MosaikMiniApp::slot_erpChanged(quint8 id, qint8 val)
{
    qDebug() <<Q_FUNC_INFO <<"erp changed:" <<"id:" <<id <<"val:" <<val;

	float change = ((float) val) / 250;

    switch(id)
    {
        case 0:
            subchannelManager().setCurrentFadeInPointRel(change);
            m_uiManager->refreshEnvelope();
            break;
        case 1:
            subchannelManager().setCurrentFadeOutPointRel(change);
            m_uiManager->refreshEnvelope();
            break;
        case 2:
            subchannelManager().setCurrentStartPointRel(change);
            m_uiManager->refreshEnvelope();
            break;
        case 3:
            subchannelManager().setCurrentEndPointRel(change);
            m_uiManager->refreshEnvelope();
            break;
        case 4:
            subchannelManager().setCurrentSubchannelVolumeRelative(change);
            //m_uiManager->refreshVolPoti();
            //m_uiManager->refreshSelectionPad();
            m_uiManager->refreshSampleVolumeWidget(subchannelManager().getCurrentSubchannelSelection());
            break;
        default:
            qDebug() <<Q_FUNC_INFO <<"Id out of range:" <<id;
            break;
    }
}




/** make obsolete */
void MosaikMiniApp::slot_subchannelChangeVolume(float relVal)
{
    qDebug() <<Q_FUNC_INFO <<"relVal" <<relVal;
    subchannelManager().setCurrentSubchannelVolumeRelative(relVal);
    m_uiManager->refreshVolPoti();
}



void MosaikMiniApp::slot_subchannelSelectionPadTriggert(int id)
{
    qDebug() <<Q_FUNC_INFO <<id;
    subchannelManager().setCurrentSubchannelSelection(id);
    subchannelManager().prelistenCurrentSubchannelSample();
    midiManager().refreshPatternView();
    midiManager().refreshSubchannelSelection();
    m_uiManager->refresh();
}



/** ****************************************************************************
    Global
*******************************************************************************/
void MosaikMiniApp::slot_globalMainVolume(float relVal)
{
    subchannelManager().setMainVolumeRelative(relVal);
    float volume = subchannelManager().getMainVolume();
    qDebug() <<Q_FUNC_INFO <<"volume" <<volume;
    midiManager().setMainVolume(volume);
    m_uiManager->refreshMainVol();
}

void MosaikMiniApp::slot_globalChangeBpmRelative(float diffVal)
{
    qDebug() <<Q_FUNC_INFO;
    subchannelManager().setBpmRelative(diffVal);
    m_alsaPcm->slot_bpmChanged(subchannelManager().getBpm());
    m_uiManager->refreshBpm();
}

void MosaikMiniApp::slot_globalPreVolume(float absVal)
{
    qDebug() <<Q_FUNC_INFO;
    subchannelManager().setPreVolumeAbs(absVal);
    m_uiManager->refreshPreVol();
}




/** ****************************************************************************
    App
*******************************************************************************/
void MosaikMiniApp::slot_setFullScreen(void)
{
    showFullScreen();
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MosaikMiniApp::slot_setNormalScreen(void)
{
    showNormal();
    this->resize(settings().getScreenSize().width(), settings().getScreenSize().height() * 0.5);

    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void MosaikMiniApp::slot_appToggleFullScreen()
{
    if( isFullScreen() )
        slot_setNormalScreen();
    else
        slot_setFullScreen();
}

void MosaikMiniApp::slot_exitApplication(void)
{
    //close();
}

void MosaikMiniApp::slot_appExit()
{
    //close();
}



void MosaikMiniApp::slot_buttonPressed(int id)
{
    switch (id)
    {
        case 0:
        case 1:
        case 2:
        case 3:
            m_uiManager->setPageIndex(id);
            break;
        case 4:
            slot_setFullScreen();
            break;
        case 5:
            slot_setNormalScreen();
            break;
        case 6:
            slot_exitApplication();
            break;
        default:
            break;
    }
}










void MosaikMiniApp::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
        case Qt::Key_F1:
        {
            slot_setNormalScreen();
            break;
        }
        case Qt::Key_F2:
        {
            slot_setFullScreen();
            break;
        }
        case Qt::Key_F5:
        {
            m_uiManager->setPageIndex(0);
            break;
        }
        case Qt::Key_F6:
        {
            m_uiManager->setPageIndex(1);
            break;
        }
        case Qt::Key_F7:
        {
            m_uiManager->setPageIndex(2);
            break;
        }
        case Qt::Key_F12:
        {
            midiManager().setStepLed(11);
            break;
        }
        case Qt::Key_1:
        {
            slot_stepButtonPressed(0);
            break;
        }
        case Qt::Key_2:
        {
            slot_stepButtonPressed(16);
            break;
        }
        case Qt::Key_3:
        {
            slot_stepButtonPressed(32);
            break;
        }
        case Qt::Key_4:
        {
            slot_stepButtonPressed(48);
            break;
        }
        case Qt::Key_5:
        {
            m_uiManager->setPageIndex(4);
            break;
        }
        case Qt::Key_6:
        {
            QByteArray pattern = subchannelManager().getCurrentChannelPattern();
            qDebug() <<Q_FUNC_INFO <<"pattern size:" <<pattern.size();

            QString pat;
            for(int i = 0; i < pattern.size(); i++)
                pat = pat + QString::number(pattern.at(i));

            qDebug() <<Q_FUNC_INFO <<"Pattern:" <<pat;
            break;
        }
        case Qt::Key_7:
        {
            midiManager().setParentWidget(this);
            midiManager().connectFavouriteDevice();
            break;
        }
        case Qt::Key_8:
        {
            m_data.clear();
            m_data.resize(192);
            qDebug() <<Q_FUNC_INFO <<"midi bytes to send:" <<m_data.size();

            for (int i = 0; i < m_data.size()/3; i++)
            {
                m_data[3*i+0] = MIDI_MSG_NOTE_ON | MIDI_CH_SEQ;
                m_data[3*i+1] = i;
                m_data[3*i+2] = 0x3;
            }

            midiManager().sendData(m_data);
            qDebug() <<Q_FUNC_INFO <<"sending finished";
            break;
        }
        case Qt::Key_9:
            subchannelManager().toggleStep(3);
            m_uiManager->refresh();
            break;
        case Qt::Key_0:
            subchannelManager().unloadSampleOfCurrentSubchannel();
            m_uiManager->refresh();
            break;
        case Qt::Key_Escape:
        {
            close();
        }
        default:
        {
            break;
        }
    }
}

