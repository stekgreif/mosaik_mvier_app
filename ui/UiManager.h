#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QWidget>
#include <QStackedWidget>
#include <QTimer>


#include "ui/pages/subchannel/PageSubchannel.h"
#include "ui/pages/selection/PageSelection.h"
#include "ui/pages/info/PageInfos.h"
#include "ui/pages/test/PageTest.h"
#include <QTabWidget>


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

        PageSubchannel *m_pageSubchannel;
        PageInfos *m_pageInfo;
        PageTest *m_pageTest;


    protected:
        void paintEvent(QPaintEvent *event);


    private:
        MosaikMiniApp *m_mosaikMiniApp;

        //QStackedWidget  *m_stackedPages;
        QTabWidget *m_stackedPages;

        /* side panels */
        QLabel              *m_labelStepCounter;
        QLabel              *m_labelBpm;
        QLabel              *m_labelMainVol;
        QLabel              *m_labelPreVol;

        /* pages */
        PageSelection  *m_pageSelection;
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
        void refreshSampleVolumeWidget(int id);
        void refreshPlayDirection(void);

        void slot_browserOpenFolder();
        void slot_browserCloseFolder();

        void slot_toggleMuteAndSolo();


        void loadSelectedSampleToCurrentSubchannel(void);
        void loadSelectedSampleToPrelisten(void);
        void slot_setPathId(int pathId);
};

#endif // UIMANAGER_H
