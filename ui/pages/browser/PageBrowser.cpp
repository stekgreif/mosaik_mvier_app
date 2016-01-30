#include "PageBrowser.h"
#include <QDebug>
#include "core/subchannel/SubchannelManager.h"
#include "ui/UiManager.h"
#include <QFileInfo>



PageBrowser::PageBrowser(UiManager *parent, QWidget *uiParent)
    : QWidget(uiParent)
    , m_parent(parent)
{
    qDebug() <<Q_FUNC_INFO <<"Init.";

    this->setObjectName("pageBrowser");

    m_fileBrowserAttributes   = new QRect( 20,  20, 650, 850);
    m_pathButtonsAttributes   = new QRect( 20, 890, 147,  70);
    m_loadSampleBtnAttributes = new QRect( 20, 980, 310,  70);



    /** buttons **/
    QString defaultPath = "/";
    m_path[0] = "/media/dan/Daten/Soundbänke";
    m_path[1] = "/media/dan/Daten/Soundbänke/Mosaik Soundbank";
    m_path[2] = "/home/dan/samples/mitCamelCase";
    m_path[3] = "/media/dan/Daten/Soundbänke/TV-Shows & Youtube/raw";

    m_pathName[0] = "Soundbänke";
    m_pathName[1] = "Mosaik";
    m_pathName[2] = "Testsamples";
    m_pathName[3] = "Youtube";

    for( int i = 0; i < 4; i++)
    {
        m_pathBtn[i] = new QPushButton(this);
        m_pathBtn[i]->setObjectName("pathButton");
        m_pathBtn[i]->setFixedSize(m_pathButtonsAttributes->size());
        m_pathBtn[i]->setText(m_pathName[i]);
        m_pathBtn[i]->move( m_pathButtonsAttributes->left() + ( i * (m_pathButtonsAttributes->width() + 20) )
                           ,m_pathButtonsAttributes->top()   );
    }


    QFont fb("Droid Sans", 14, QFont::Bold);
    m_btnLoadCurSample = new QPushButton(this);
    m_btnLoadCurSample->setFont(fb);
    m_btnLoadCurSample->setFixedSize(m_loadSampleBtnAttributes->size());
    m_btnLoadCurSample->move(m_loadSampleBtnAttributes->topLeft());
    m_btnLoadCurSample->setText("Load Sample");


    m_changePage = new QPushButton(this);
    m_changePage->move(710,945);
    m_changePage->setFixedSize(80,80);


    /** file browser **/
    m_treeView = new SampleBrowser(this);
    m_treeView->move(m_fileBrowserAttributes->topLeft());
    m_treeView->setFixedSize(m_fileBrowserAttributes->size());
    m_fileSystem = new QFileSystemModel(this);
    m_fileSystem->setFilter( QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files );
    m_fileSystem->setNameFilters( QStringList() <<"*.wav" <<"*.WAV" );
    m_fileSystem->setNameFilterDisables(false); // hide other files than wav and do not gray them out
    m_fileSystem->setRootPath(defaultPath);
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




    /** connects **/
    connect( m_btnLoadCurSample, SIGNAL(clicked()), this, SLOT(slot_loadSample())            );
    connect( m_pathBtn[0],       SIGNAL(clicked()), this, SLOT(slot_changePath0())           );
    connect( m_pathBtn[1],       SIGNAL(clicked()), this, SLOT(slot_changePath1())           );
    connect( m_pathBtn[2],       SIGNAL(clicked()), this, SLOT(slot_changePath2())           );
    connect( m_pathBtn[3],       SIGNAL(clicked()), this, SLOT(slot_changePath3())           );
    connect( m_changePage,       SIGNAL(pressed()), this, SLOT(slot_changePage())            );
}

PageBrowser::~PageBrowser()
{

}

void PageBrowser::refresh()
{

}



void PageBrowser::slot_loadSample(void)
{
    QFileInfo fileInfo(m_fileSystem->filePath(m_treeView->currentIndex()));

    if( fileInfo.isFile() && fileInfo.exists() )
    {
        subchannelManager().loadSampleToCurrentSubchannel(m_fileSystem->filePath(m_treeView->currentIndex()));
        m_parent->refreshSelection();
        emit signal_btnLoadSamplePressed();
    }
    else
        qDebug() <<Q_FUNC_INFO <<"Not a file.";

}

void PageBrowser::slot_loadSampleToPrelisten()
{
    QFileInfo fileInfo(m_fileSystem->filePath(m_treeView->currentIndex()));

    if( fileInfo.isFile() && fileInfo.exists() )
        subchannelManager().loadSampleToPrelisten(m_fileSystem->filePath(m_treeView->currentIndex()));
    else
        qDebug() <<Q_FUNC_INFO <<"Not a file.";
}



void PageBrowser::slot_toggleItmeExpansion()
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



void PageBrowser::slot_changePage()
{
    m_parent->setPageIndex(0);
}



void PageBrowser::slot_moveCursor(int direction)
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





void PageBrowser::slot_changePathId(int pathId)
{
    switch (pathId)
    {
        case 0:
            slot_changePath0();
            break;
        case 1:
            slot_changePath1();
            break;
        case 2:
            slot_changePath2();
            break;
        case 3:
            slot_changePath3();
            break;
        default:
            qDebug() <<Q_FUNC_INFO <<"Path id:" <<pathId <<"not in range.";
            break;
    }
}


void PageBrowser::slot_changePath0(void)
{
    m_treeView->setRootIndex(m_fileSystem->setRootPath(m_path[0]));
}

void PageBrowser::slot_changePath1(void)
{
    m_treeView->setRootIndex(m_fileSystem->setRootPath(m_path[1]));
}

void PageBrowser::slot_changePath2(void)
{
    m_treeView->setRootIndex(m_fileSystem->setRootPath(m_path[2]));
}

void PageBrowser::slot_changePath3(void)
{
    m_treeView->setRootIndex(m_fileSystem->setRootPath(m_path[3]));
}


