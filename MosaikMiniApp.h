#ifndef MOSAIKMINIAPP_H
#define MOSAIKMINIAPP_H

#include <QMainWindow>
#include <QScrollArea>
#include <QWidget>
#include <QDesktopWidget>
#include <QStackedWidget>
#include "ui/UiManager.h"
#include "ui/pages/subchannel/PageSubchannel.h"
#include "ui/pages/selection/PageSelection.h"
#include "core/subchannel/SubchannelManager.h"
#include "core/audio/AlsaPcm.h"
#include "core/midi/MidiInfo.h"
#include "core/midi/MidiIn.h"
#include <QTimer>


namespace Ui
{
    class MosaikMiniApp;
}


class MosaikMiniApp : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MosaikMiniApp(QWidget *parent = 0);
        ~MosaikMiniApp();

    public slots:
        #if 1 // old slots
        void slot_buttonPressed(int id);
        void slot_subchannelSelectionPadTriggert(int id);

        void slot_setFullScreen(void);
        void slot_setNormalScreen(void);
        void slot_exitApplication(void);

        void slot_seqButtonChanged(quint8 id, quint8 val);
        void slot_fnlButtonChanged(quint8 id, quint8 val);
        void slot_fnrButtonChanged(quint8 id, quint8 val);
        void slot_sparkEvent(quint8 id, int val);

        void slot_stepButtonPressed(int id);

        void slot_erpChanged(quint8 id, qint8 val);

        void slot_regularTimer();
        void slot_subchannelChangeVolume(float relVal);
        #endif

    /** new unified slots **/
        void slot_appExit(void);
        void slot_appToggleFullScreen(void);

        void slot_sampleUnloadCurrentSubchannel(void);
        void slot_sampleUnloadCurrentChannel(void);
        void slot_sampleUnloadAll(void);
        void slot_sampleLoadToCurrentSubchannel(void);
        void slot_sampleLoadToPrelisten(void);
        void slot_prelistenSubchannelSample(void);

        void slot_patternClearCurrentSubchannel(void);
        void slot_patternClearCurrentChannel(void);
        void slot_patternClearAll(void);

        void slot_selectionSetCurrentSubchannelRelative(int id);
        void slot_selectionSetCurrentChannelRelative(int relVal);
        void slot_selectionSetCurrentChannel(int id);
        void slot_selectionNextSubchannel();

        void slot_uiSetToPageSubchannel(void);
        void slot_uiSetToPageBrowser(void);

        void slot_envelopeChangeCurrentSubchFadeIn(float relVal);
        void slot_envelopeChangeCurrentSubchStart(float relVal);
        void slot_envelopeChangeCurrentSubchEnd(float relVal);
        void slot_envelopeChangeCurrentSubchFadeOut(float relVal);

        void slot_browserChangePathId(int id);
        void slot_browserChangeCursorPosition(int direction);
        void slot_browserToggleFolderExpansion(void);
        void slot_browserSelectedSampleToPrelisten(void);
        void slot_browserOpenFolder(void);
        void slot_browserCloseFolder(void);

        void slot_globalMainVolume(float relVal);
        void slot_globalChangeBpmRelative(float diffVal);
        void slot_globalPreVolume(float absVal);

        void slot_parameterChangeCurrentSubchVolume(float relVal);
        void slot_parameterPan(float relVal);
        void slot_parameterCurrentSubToPre(bool state);
        void slot_parameterMuteAndSolo(bool state);
        void slot_parameterUnmuteAll(void);
        void slot_parameterSelectLastMutes(void);
        void slot_parameterPlayDirection(bool direction);

    private:
        Ui::MosaikMiniApp *ui;
        UiManager *m_uiManager;
        QScrollArea *m_scrollArea;
        QDesktopWidget *m_screenInfo;
        QRect m_screenSize;
        QByteArray m_data;
        MidiInfo *m_midiInfo;
        AlsaPcm *m_alsaPcm;
        SubchannelManager *m_subchannelManager;
        void keyPressEvent(QKeyEvent* event);
        QTimer *m_timer;
        quint32 m_stepCounter;
};

#endif // MOSAIKMINIAPP_H
