#include "Browser.h"
#include <QDebug>
#include "core/subchannel/SubchannelManager.h"



Browser::Browser(QWidget *uiParent)
    : QWidget(uiParent)
{
    qDebug() <<Q_FUNC_INFO <<"Init.";

    /** change size and position here **/
    m_browserWindowSizeAndPosition = new QRect( 20,  20, 600, 650);


    /** path **/
    m_defaultPath = "/";
    m_path[0] = "/media/dan/Daten/Soundbänke";
    m_path[1] = "/media/dan/Daten/Soundbänke/Mosaik Soundbank";
    m_path[2] = "/home/dan/samples/mitCamelCase";
    m_path[3] = "/media/dan/Daten/Soundbänke/TV-Shows & Youtube/raw";
    m_pathName[0] = "Soundbänke";
    m_pathName[1] = "Mosaik";
    m_pathName[2] = "Testsamples";
    m_pathName[3] = "Youtube";

    /** file browser **/
    m_treeView = new BrowserTree(this);
    //m_treeView->move(m_browserWindowSizeAndPosition->topLeft());
    m_treeView->setFixedSize(m_browserWindowSizeAndPosition->size());
    m_fileSystem = new QFileSystemModel(this);
    m_fileSystem->setFilter( QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files );
    m_fileSystem->setNameFilters( QStringList() <<"*.wav" <<"*.WAV" );
    m_fileSystem->setNameFilterDisables(false); // hide other files than wav and do not gray them out
    m_fileSystem->setRootPath(m_defaultPath);
    m_treeView->setObjectName("treeView");
    m_treeView->setFocus();
    m_treeView->setModel(m_fileSystem);
    m_treeView->setAutoScroll(true);  // default
    m_treeView->setColumnWidth(0, 500);
    m_treeView->hideColumn(3);  // 0:name, 1:size, 2:type, 3:date modified
    m_treeView->hideColumn(2);
    QFont treeFont;
    treeFont.setPixelSize(14);
    m_treeView->setFont(treeFont);
}

Browser::~Browser()
{

}



void Browser::slot_changePath(int pathId)
{
    if( (pathId < 4) && (pathId >= 0) )
    {
        qDebug() <<Q_FUNC_INFO <<"PathId valide.";
        m_treeView->setRootIndex(m_fileSystem->setRootPath(m_path[pathId]));
    }
    else
        qDebug() <<Q_FUNC_INFO <<"PathId out of range.";
}

void Browser::slot_changePath(QString pathName)
{
    Q_UNUSED(pathName);
    qDebug() <<Q_FUNC_INFO <<"Not implemented yet.";
}




void Browser::slot_moveCursor(int direction)
{
    switch (direction)
    {
        case -1:
            m_treeView->cursorDown();
            break;
        case 1:
            m_treeView->cursorUp();
            break;
        default:
            break;
    }
}



void Browser::slot_toggleItmeExpansion()
{
    QFileInfo fileInfo(m_fileSystem->filePath(m_treeView->currentIndex()));

    if( fileInfo.isFile() )
    {
        qDebug() <<Q_FUNC_INFO <<"Item is a file => Load sample.";
        slot_loadSample();
    }
    else if ( fileInfo.isDir() )
    {
        qDebug() <<Q_FUNC_INFO <<"Item is a directory => Toggle expand.";
        m_treeView->setExpanded( m_treeView->currentIndex(), !m_treeView->isExpanded(m_treeView->currentIndex()) );
    }
    else
        qDebug() <<Q_FUNC_INFO <<"Item is unknown.";
}



void Browser::slot_loadSample(void)
{
    QFileInfo fileInfo(m_fileSystem->filePath(m_treeView->currentIndex()));

    if( fileInfo.isFile() && fileInfo.exists() )
    {
        subchannelManager().loadSampleToCurrentSubchannel(m_fileSystem->filePath(m_treeView->currentIndex()));
        //m_parent->refreshSelection();
        //emit signal_btnLoadSamplePressed();
    }
    else
        qDebug() <<Q_FUNC_INFO <<"Not a file.";

}

void Browser::slot_loadSampleToPrelisten()
{
    QFileInfo fileInfo(m_fileSystem->filePath(m_treeView->currentIndex()));

    if( fileInfo.isFile() && fileInfo.exists() )
        subchannelManager().loadSampleToPrelisten(m_fileSystem->filePath(m_treeView->currentIndex()));
    else
        qDebug() <<Q_FUNC_INFO <<"Not a file.";
}

