#ifndef BROWSER_H
#define BROWSER_H

#include <QWidget>
#include <QDirModel>
#include <QFileSystemModel>
#include <QLabel>

#include "ui/pages/subchannel/BrowserTree.h"



class UiManager;

class Browser : public QWidget
{
    Q_OBJECT

    public:
        explicit Browser(QWidget *uiParent = 0);
        ~Browser();

    public slots:
        void slot_changePath(int pathId);
        void slot_changePath(QString pathName);
        void slot_moveCursor(int direction);
        void slot_toggleItmeExpansion(void);
        void slot_loadSample(void);
        void slot_loadSampleToPrelisten(void);
        void slot_toggleParentFolderState(void);
        void slot_openFolder(void);
        void slot_closeFolder(void);


    private:
        /** path and file system **/
        QFileSystemModel *m_fileSystem;
        BrowserTree      *m_treeView;
        QRect *m_browserWindowSizeAndPosition;

        QString m_defaultPath;
        QString m_path[10];
        QString m_pathName[10];

};

#endif // BROWSER_H
