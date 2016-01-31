#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QWidget>
#include <QStackedWidget>
#include <QTimer>

#include <ui/sidepanels/SubchannelPreview.h>

#include "ui/pages/debug/PageDebug.h"
#include "ui/pages/subchannel/PageSubchannel.h"
#include "ui/pages/selection/PageSelection.h"
#include "ui/pages/settings/PageSettings.h"
#include "ui/pages/info/PageInfos.h"
#include "ui/pages/browser/PageBrowser.h"


class MosaikMiniApp;

class UiManager : public QWidget
{
    Q_OBJECT

    public:
        //explicit UiManager(MosaikMiniApp *parent = 0, QWidget *uiParent = 0);
        explicit UiManager(MosaikMiniApp *m_mosaikMiniApp = 0, QWidget *uiParent = 0);
        ~UiManager();

        void setPageIndex(int id);
        int  getCurrentPageId(void);
        MosaikMiniApp *getParent(void);
        PageBrowser    *m_pageBrowser;
        PageSubchannel *m_pageSubchannel;


    protected:
        void paintEvent(QPaintEvent *event);


    private:
        MosaikMiniApp *m_mosaikMiniApp;

        QStackedWidget  *m_stackedPages;
        //QStackedWidget  *m_overlayStack;

        /* side panels */
        SubchannelPreview   *m_subchannelPreview;
        QLabel              *m_labelStepCounter;
        QLabel              *m_labelBpm;
        QLabel              *m_labelMainVol;
        QLabel              *m_labelPreVol;

        /* pages */
        PageDebug      *m_pageDebug;
        PageSelection  *m_pageSelection;
        PageSettings   *m_pageSettings;
        PageInfos      *m_pageInfos;

        QTimer *m_timer;





    signals:
        void signal_subchannelSelectionPadTriggert(int id);


    private slots:
        void slot_subchannelSelectionPadTriggert(int id);
        void slot_regularTimer(void);


    public slots:
        void refresh(void);
        void refreshSelection(void);
        void refreshEnvelope(void);
        void refreshStepCounterAbsolute(quint64 cnt);
        void refreshVolPoti(void);
        void refreshBpm(void);
        void refreshPreVol(void);
        void refreshMainVol(void);
        void refreshMutePads(void);
        void refreshSelectionPad(void);

        void slot_toggleMuteAndSolo();


        void loadSelectedSampleToCurrentSubchannel(void);
        void loadSelectedSampleToPrelisten(void);
        void slot_setPathId(int pathId);
};

#endif // UIMANAGER_H
