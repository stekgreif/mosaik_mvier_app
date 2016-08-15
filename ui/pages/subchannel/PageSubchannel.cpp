#include "PageSubchannel.h"

#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QPainter>
#include <QStyleOption>

#include "ui/pages/subchannel/qcustomplot.h"
#include "core/subchannel/SubchannelManager.h"
#include <ui/pages/subchannel/Pattern.h>
#include "MosaikTypes.h"
#include "ui/UiManager.h"
#include "MosaikMiniApp.h"



PageSubchannel::PageSubchannel(MosaikMiniApp *mosaikMiniApp, UiManager *parent)
    : m_parent(parent)
    , m_mosaikMiniApp(mosaikMiniApp)
{

    qDebug() <<Q_FUNC_INFO <<"Init.";
    this->setObjectName("pageSubchannel");
    QFont labelFont("Droid Sans", 12);
    QFont varLabelFont("Droid Sans Mono", 12);
    m_leftChannelPen.setColor(  QColor(0,0,255) );
    m_rightChannelPen.setColor( QColor(255,0,0,10) );


    /** ui layout -- use these to change to position of widgets on the page - xPos, yPos, width, height **/
    m_sampleWindowAttributes  = new QRect(  20,  20, 780, 300 );
    m_sampleLabelAttributes   = new QPoint(635, 350);
    m_patternWidgetAttributes = new QRect( 635, 600, 400, 400 );


    /** sample value labels **/
    m_labelVarPathAndName = new QLabel("-", this);
    m_labelVarPathAndName->setFont(varLabelFont);
    m_labelVarPathAndName->setObjectName("labelVarPathAndName");
    m_labelVarPathAndName->setStyleSheet("QLabel#labelVarPathAndName {color: rgb(200,200,200)};");
    m_labelVarPathAndName->move(m_sampleWindowAttributes->left(), m_sampleWindowAttributes->top()-20); //360,160


    /** BROWSER **/
    m_browser = new Browser(this);
    m_browser->move(5,415); //2016-08-10 change browser pos here


    m_btnTestPlayDirection = new QPushButton(this);
    m_btnTestPlayDirection->setFixedSize(50,50);
    m_btnTestPlayDirection->move(10,340);
    m_btnTestPlayDirection->setText(">");

    m_pixmap = new QPixmap;
    m_pixmap->fill(Qt::white);

    m_labelImage = new QLabel(this);
    m_labelImage->move(20, 20);
    m_labelImage->setFixedSize(780,300);
    m_labelImage->setObjectName("labelImage");
    m_labelImage->setStyleSheet("QLabel#labelImage {background-color: rgb(100,100,120);}");
    m_labelImage->setPixmap(*m_pixmap);


    /** envelope **/
    QSize envSize;
    const int frameSize = 0;
    envSize.setHeight(m_sampleWindowAttributes->height());
    envSize.setWidth(m_sampleWindowAttributes->width() - frameSize);
    m_envelope = new DrawEnvelope(envSize, this);
    //m_envelope->move(m_sampleWindowAttributes->topLeft());
    m_envelope->move(m_sampleWindowAttributes->left(), m_sampleWindowAttributes->top());
    m_envelope->setFixedHeight(m_sampleWindowAttributes->height());
    m_envelope->setFixedWidth(m_sampleWindowAttributes->width());


    m_btnLoadSample = new QPushButton(this);
    m_btnLoadSample->move(120,340);
    m_btnLoadSample->setFixedSize(50,50);
    m_btnLoadSample->setText("load \n sample");

    m_pattern = new Pattern(m_mosaikMiniApp, this);

    connect( m_btnLoadSample,
             SIGNAL(clicked()),
             this,
             SLOT(slot_loadSample())            );

    connect( m_btnTestPlayDirection,
             SIGNAL(clicked()),
             this,
             SLOT(slot_togglePlayDirection()));

    refresh();
}

PageSubchannel::~PageSubchannel()
{

}



void PageSubchannel::refreshSamplePlot()
{
    if( subchannelManager().hasCurrentSubchannelSample() )
    {
        qDebug() <<Q_FUNC_INFO  <<"Refresh sample plot.";

        QSharedPointer<Sample> samplePtr = subchannelManager().getSharedPointerToSample();
        m_labelImage->setPixmap(*samplePtr->getSampleStructPointer()->pixmap);
    }
    else
    {
        resetSamplePlot();
    }
}

void PageSubchannel::resetSamplePlot()
{
    m_labelImage->setPixmap(*m_pixmap);
}



void PageSubchannel::slot_changePage(void)
{
    m_parent->setPageIndex(1);
}

void PageSubchannel::slot_togglePlayDirection()
{
    subchannelManager().togglePlayDirection();
}



void PageSubchannel::refresh()
{
    refreshSamplePlot();

    QSharedPointer<Sample> samplePtr = subchannelManager().getSharedPointerToSample();
    if(samplePtr != NULL)
    {
        qDebug() <<Q_FUNC_INFO <<"Sample is loaded.";
        m_labelVarPathAndName->setText(samplePtr->getSampleStructPointer()->pathAndName);
        m_labelVarPathAndName->adjustSize();
    }

    m_envelope->setEnvelope(subchannelManager().getCurrentEnvelope());

    refreshPots();
}



void PageSubchannel::refreshEnvelope()
{
    m_envelope->setEnvelope(subchannelManager().getCurrentEnvelope());
    refreshLabels();
}

void PageSubchannel::refreshPots()
{
    qDebug() <<Q_FUNC_INFO <<"delete me";
}



void PageSubchannel::refreshLabels()
{
    qDebug() <<Q_FUNC_INFO <<"delete me";
#if 0 // remove canidate
    QSharedPointer<Sample> samplePtr = subchannelManager().getSharedPointerToSample();

    if(samplePtr != NULL)
    {
        envelope_t envelope = subchannelManager().getCurrentEnvelope();
        int nof = samplePtr->getSampleStructPointer()->sndInfo.frames;
        int fs  = samplePtr->getSampleStructPointer()->sndInfo.samplerate;
        int startMs = ( (envelope.start * nof) / fs) * 1000;
        int endMs   = ( (envelope.end   * nof) / fs) * 1000;
        int framesPerStep = (60 * fs) / (4 * subchannelManager().getBpm());
        float steps = 40 * ((float)(endMs - startMs) / (float)framesPerStep);

        m_samplePropertyLabel->at( SampleInfoIds::channels  )->setText( QString::number( samplePtr->getSampleStructPointer()->sndInfo.channels ));
        m_samplePropertyLabel->at( SampleInfoIds::frames    )->setText( QString::number( nof ) );
        m_samplePropertyLabel->at( SampleInfoIds::samplerate)->setText( QString::number( fs ) + " Hz" );
        m_samplePropertyLabel->at( SampleInfoIds::length    )->setText( QString::number( (int)samplePtr->getSampleStructPointer()->lengthInMs ) + " ms" );
        m_samplePropertyLabel->at( SampleInfoIds::start     )->setText( QString::number( startMs )         + " ms" );
        m_samplePropertyLabel->at( SampleInfoIds::end       )->setText( QString::number( endMs )           + " ms" );
        m_samplePropertyLabel->at( SampleInfoIds::delta     )->setText( QString::number( endMs - startMs ) + " ms" );
        m_samplePropertyLabel->at( SampleInfoIds::steps     )->setText( QString::number( steps, 'f', 2));
        m_samplePropertyLabel->at( SampleInfoIds::volume    )->setText( QString::number( subchannelManager().getCurrentSubchannelVolume(), 'f', 2 ) );
        m_samplePropertyLabel->at( SampleInfoIds::pan       )->setText( QString::number( subchannelManager().getCurrentPan(), 'f', 2 ) );

        for( int i = 0; i < 10; i++ )
            m_samplePropertyLabel->at(i)->adjustSize();
    }
    else
    {
        resetSamplePlot();
    }
#endif
}



void PageSubchannel::refreshStepAxis()
{
    if( subchannelManager().hasCurrentSubchannelSample() )
    {
        qDebug() <<Q_FUNC_INFO  <<"Not implemented yet.";
    }
}


/** browser slots **/
void PageSubchannel::slot_changePath(int pathId)
{
    m_browser->slot_changePath(pathId);
}

void PageSubchannel::slot_changePath(QString pathName)
{
    m_browser->slot_changePath(pathName);
}

void PageSubchannel::slot_moveCursor(int direction)
{
    m_browser->slot_moveCursor(direction);
}

void PageSubchannel::slot_toggleItmeExpansion()
{
    m_browser->slot_toggleItmeExpansion();
}

void PageSubchannel::slot_loadSample()
{
    m_browser->slot_loadSample();
}

void PageSubchannel::slot_loadSampleToPrelisten()
{
    m_browser->slot_loadSampleToPrelisten();
}

void PageSubchannel::slot_browserOpenFolder()
{
    m_browser->slot_openFolder();
}

void PageSubchannel::slot_browserCloseFolder()
{
    m_browser->slot_closeFolder();
}

void PageSubchannel::slot_refreshPlayDirection()
{
    if( subchannelManager().getCurrentPlayDirection() )
    {
        m_btnTestPlayDirection->setText(">");
    }
    else
    {
        m_btnTestPlayDirection->setText("<");
    }
}



void PageSubchannel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption myOption;
    myOption.initFrom(this);
    QPainter myPainter(this);
    style()->drawPrimitive( QStyle::PE_Widget, &myOption, &myPainter,this);
}

