#ifndef BROWSER_H
#define BROWSER_H

#include <QWidget>
#include <QDirModel>
#include <QTreeView>
#include <QPushButton>
#include <QFileSystemModel>
#include <QToolButton>
#include "SampleBrowser.h"


class UiManager;

class PageBrowser : public QWidget
{
    Q_OBJECT

    public:
        explicit PageBrowser(UiManager *parent, QWidget *uiParent = 0);
        ~PageBrowser();
        void refresh(void);

    signals:
        void signal_btnLoadSamplePressed(void);


    public slots:
        void slot_changePath0(void);
        void slot_changePath1(void);
        void slot_changePath2(void);
        void slot_changePath3(void);

        void slot_changePathId(int pathId);

        void slot_moveCursor(int direction);
        void slot_loadSample(void);

        void slot_loadSampleToPrelisten(void);

        void slot_toggleItmeExpansion(void);

    private slots:
        void slot_changePage(void);

    private:
        UiManager *m_parent;

        /** path and file system **/
        QFileSystemModel *m_fileSystem;
        SampleBrowser    *m_treeView;
        //QString          *m_userPath;

        /** buttons **/
        QPushButton *m_changePage;
        QPushButton *m_btnLoadCurSample;
        QString m_path[4];
        QString m_pathName[4];
        QPushButton *m_pathBtn[4];

        QRect *m_fileBrowserAttributes;
        QRect *m_pathButtonsAttributes;
        QRect *m_loadSampleBtnAttributes;
};

#endif // BROWSER_H
