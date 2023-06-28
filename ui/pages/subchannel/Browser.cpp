#include "Browser.h"
#include <QDebug>
#include "core/subchannel/SubchannelManager.h"
#include "Settings.h"


Browser::Browser(QWidget *uiParent)
    : QWidget(uiParent)
{
    qDebug() <<Q_FUNC_INFO <<"Init.";

	/** size of actual screen **/
	int screenHeight = settings().getScreenSize().height();
	int screenWidth = settings().getScreenSize().width();

    /** path **/
    m_defaultPath = "/mnt/ad4247cf-f066-4f4c-a5f3-3ab1a55586e0/SAMPLES";
    m_path[0] = "/mnt/ad4247cf-f066-4f4c-a5f3-3ab1a55586e0/SAMPLES";
    m_path[1] = "/media/dan/Daten/Soundbänke/Mosaik4 Soundbank/vocal";
    m_path[2] = "/media/dan/Daten/Soundbänke/Mosaik4 Soundbank/fx";
    m_path[3] = "/media/dan/Daten/Soundbänke/Mosaik4 Soundbank/tonal";
    m_path[4] = "/media/dan/Daten/Soundbänke/Mosaik4 Soundbank/ambient";
    m_path[5] = "/media/dan/Daten/Soundbänke/Mosaik4 Soundbank/bass";
    m_path[6] = "/media/dan/Daten/Soundbänke/Mosaik4 Soundbank/noise";
    m_path[7] = "/media/dan/Daten/Soundbänke/Mosaik4 Soundbank/drums";
    m_path[8] = "/media/dan/Daten/Soundbänke/Mosaik Soundbank";
    m_pathName[0] = "Instuments";
    m_pathName[1] = "Vocal";
    m_pathName[2] = "Fx";
    m_pathName[3] = "Tonal";
    m_pathName[4] = "Ambient";
    m_pathName[5] = "Bass";
    m_pathName[6] = "Noise";
    m_pathName[7] = "Drums";
    m_pathName[8] = "Mosaik Soundbank";

    /** file browser **/
    m_treeView = new BrowserTree(this);
	m_treeView->setFixedSize( 40* screenWidth/100, 63* screenHeight/100 );

    m_fileSystem = new QFileSystemModel(this);
    m_fileSystem->setFilter( QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files );
    m_fileSystem->setNameFilters( QStringList() <<"*.wav" <<"*.WAV" );
    m_fileSystem->setNameFilterDisables(false); // hide other files than wav and do not gray them out
    m_fileSystem->setRootPath(m_defaultPath);

	m_treeView->setObjectName("treeView");
    m_treeView->setStyleSheet("QTreeView#treeView {background-color: rgb(90,90,90);}");
    m_treeView->setFocus();
    m_treeView->setModel(m_fileSystem);
    m_treeView->setAutoScroll(true);  // default
    m_treeView->setColumnWidth(0, 480);
    m_treeView->hideColumn(3);  // 0:name, 1:size, 2:type, 3:date modified
    m_treeView->hideColumn(2);
    QFont treeFont;
    treeFont.setPixelSize(14);
    m_treeView->setFont(treeFont);

    slot_changePath( 0 );

    connect( m_treeView, SIGNAL( signal_enterPressed() ), this, SLOT( slot_loadSample ) );
}


Browser::~Browser()
{

}


void Browser::slot_changePath(int pathId)
{
    if( (pathId < 9) && (pathId >= 0) )
    {
        qDebug() <<Q_FUNC_INFO <<"PathId valide:" <<pathId <<":" <<m_path[pathId];
        m_treeView->setRootIndex(m_fileSystem->setRootPath(m_path[pathId]));
    }
    else
    {
        qDebug() <<Q_FUNC_INFO <<"PathId out of range." <<pathId <<"Path set to default";
        m_treeView->setRootIndex(m_fileSystem->setRootPath(m_defaultPath));
    }
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
        //slot_loadSample();
    }
    else if ( fileInfo.isDir() )
    {
        qDebug() <<Q_FUNC_INFO <<"Item is a directory => Toggle expand.";
        m_treeView->setExpanded( m_treeView->currentIndex(), !m_treeView->isExpanded(m_treeView->currentIndex()) );
    }
    else
    {
        qDebug() <<Q_FUNC_INFO <<"Item is unknown.";
    }
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
    {
        qDebug() <<Q_FUNC_INFO <<"Not a file.";
    }

}


void Browser::slot_loadSampleToPrelisten()
{
    QFileInfo fileInfo(m_fileSystem->filePath(m_treeView->currentIndex()));

    if( fileInfo.isFile() && fileInfo.exists() )
    {
        subchannelManager().loadSampleToPrelisten(m_fileSystem->filePath(m_treeView->currentIndex()));
    }
    else
    {
        qDebug() <<Q_FUNC_INFO <<"Not a file.";
    }
}


void Browser::slot_toggleParentFolderState()
{
    //m_treeView->cursorLeft();

    //qDebug() <<Q_FUNC_INFO <<m_treeView->currentIndex();


    /** closing mechanism **/
    QModelIndex curIdx = m_treeView->currentIndex();
    qDebug() <<Q_FUNC_INFO <<"start:" <<curIdx.row();

    if(curIdx.row() == 0)
    {
        m_treeView->cursorUp();
        m_treeView->setExpanded(m_treeView->currentIndex(), false);
    }
    else
    {
        for(int i = curIdx.row(); i >= 0; i--)
        {
            m_treeView->cursorUp();
            curIdx = m_treeView->currentIndex();
            qDebug() <<Q_FUNC_INFO <<curIdx.row();
        }

        m_treeView->setExpanded(m_treeView->currentIndex(), false);
    }

    /** @todo this can replace the BrowserTree implementation **/
    //m_treeView->setExpanded(m_treeView->currentIndex(), false);
}


void Browser::slot_openFolder()
{
    //m_treeView->setExpanded( m_treeView->currentIndex(), !m_treeView->isExpanded(m_treeView->currentIndex()) );
    m_treeView->setExpanded( m_treeView->currentIndex(), true  );
}


void Browser::slot_closeFolder()
{
    QModelIndex curIdx = m_treeView->currentIndex();
    //qDebug() <<Q_FUNC_INFO <<"start:" <<curIdx.row();

    if(curIdx.row() == 0)
    {
        m_treeView->cursorUp();
        m_treeView->setExpanded(m_treeView->currentIndex(), false);
    }
    else
    {
        for(int i = curIdx.row(); i >= 0; i--)
        {
            m_treeView->cursorUp();
            curIdx = m_treeView->currentIndex();
            //qDebug() <<Q_FUNC_INFO <<curIdx.row();
        }

        m_treeView->setExpanded(m_treeView->currentIndex(), false);
    }
}

