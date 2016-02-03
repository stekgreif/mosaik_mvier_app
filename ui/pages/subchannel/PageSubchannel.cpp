#include "PageSubchannel.h"

#include <QDebug>
#include <QLabel>
#include <QWidget>
#include <QPainter>
#include <QStyleOption>

#include "ui/pages/subchannel/qcustomplot.h"
#include "core/subchannel/SubchannelManager.h"
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
    //m_browserWidgetAttributes = new QRect(  20, 350, 600, 600 );



    /** setup sample waveform plot **/
    m_samplePlot = new QCustomPlot(this);
    m_samplePlot->setBackground(Qt::lightGray);
    m_samplePlot->axisRect()->setBackground(Qt::darkGray);
    m_samplePlot->move(m_sampleWindowAttributes->topLeft());
    m_samplePlot->setFixedSize(m_sampleWindowAttributes->width(), m_sampleWindowAttributes->height());
    m_samplePlot->yAxis->setTickLabels(false);
    m_samplePlot->yAxis->setTicks(false);


    /** sample value labels **/
    m_labelVarPathAndName = new QLabel("-", this);
    m_labelVarPathAndName->setFont(varLabelFont);
    m_labelVarPathAndName->setObjectName("labelVarPathAndName");
    m_labelVarPathAndName->setStyleSheet("QLabel#labelVarPathAndName {color: rgb(200,200,200)};");
    m_labelVarPathAndName->move(m_sampleWindowAttributes->left(), m_sampleWindowAttributes->top()-20); //360,160


    /** envelope **/
    QSize envSize;
    const int frameSize = 30;
    envSize.setHeight(m_sampleWindowAttributes->height());
    envSize.setWidth(m_sampleWindowAttributes->width() - frameSize);
    m_envelope = new DrawEnvelope(envSize, this);
    //m_envelope->move(m_sampleWindowAttributes->topLeft());
    m_envelope->move(m_sampleWindowAttributes->left() + 15, m_sampleWindowAttributes->top());
    m_envelope->setFixedHeight(m_sampleWindowAttributes->height());
    m_envelope->setFixedWidth(m_sampleWindowAttributes->width());


    /** BROWSER **/
    m_browser = new Browser(this);
    m_browser->move(20,350);
    //m_browser->setFixedSize(m_sampleWindowAttributes->width(), m_sampleWindowAttributes->height());

    /** test **/
    m_btnTestTreeCollapse = new QPushButton(this);
    m_btnTestTreeCollapse->setFixedSize(100,50);
    m_btnTestTreeCollapse->move(20,980);
    m_btnTestTreeCollapse->setText("Close Folder");

    m_btnTestPlayDirection = new QPushButton(this);
    m_btnTestPlayDirection->setFixedSize(100,50);
    m_btnTestPlayDirection->move(160,980);
    m_btnTestPlayDirection->setText("Tgl Play Dir");

    connect(m_btnTestTreeCollapse,
            SIGNAL(clicked()),
            m_browser,
            SLOT(slot_toggleParentFolderState()));

    connect(m_btnTestPlayDirection,
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


        /** get sample pointer **/
        QSharedPointer<Sample> samplePtr = subchannelManager().getSharedPointerToSample();
        int numOfFrames = samplePtr->getSampleStructPointer()->sndInfo.frames;
        int sampleRate = samplePtr->getSampleStructPointer()->sndInfo.samplerate;
        float *sampleBuffer = samplePtr->getSampleStructPointer()->frameBuffer;


        /** load sample to plot buffer **/
        QVector<double> xL(numOfFrames);
        QVector<double> yL(numOfFrames);
        QVector<double> xR;
        QVector<double> yR;

        if( (samplePtr->getSampleStructPointer()->sndInfo.channels) == mono )
        {
            for (int i=0; i<numOfFrames-1; i++)
            {
                xL[i] = i;
                yL[i] = sampleBuffer[i];
            }
        }
        else if( (samplePtr->getSampleStructPointer()->sndInfo.channels) == stereo )
        {
            xR.resize(numOfFrames);
            yR.resize(numOfFrames);

            for (int i=0; i<numOfFrames-1; )
            {
                xL[i] = i;
                yL[i] = sampleBuffer[i];
                i++;
                xR[i] = i;
                yR[i] = sampleBuffer[i];
                i++;
            }
        }


        /** axis **/
        QVector<double>  x1Ticks;
        QVector<QString> x1Label;
        for( int i = 0; i <= 10; i++ )
        {
            x1Ticks << (double)numOfFrames * 0.1 * (double)i;
            int value = 1.0/sampleRate * numOfFrames * i * 100;
            x1Label << QString::number( value );
        }

        QVector<double>  x2Ticks;
        QVector<QString> x2Label;
        int fs = samplePtr->getSampleStructPointer()->sndInfo.samplerate;
        int framesPerStep = (60 * fs) / (4 * subchannelManager().getBpm());
        int steps = 40 * ((numOfFrames) / framesPerStep);
        for( int i = 0; i <= steps; i++ )
        {
            x2Ticks << framesPerStep * i;
            x2Label << QString::number( i );
        }

        m_samplePlot->xAxis->setVisible(true);
        m_samplePlot->xAxis->setRange(0, numOfFrames);
        m_samplePlot->yAxis->setRange(-1.001, 1.001);
        m_samplePlot->xAxis->setAutoTicks(false);
        m_samplePlot->xAxis->setAutoTickLabels(false);
        m_samplePlot->xAxis->setTickVector(x1Ticks);
        m_samplePlot->xAxis->setTickVectorLabels(x1Label);

        m_samplePlot->xAxis2->setVisible(true);
        m_samplePlot->xAxis2->setRange(0, numOfFrames);
        m_samplePlot->yAxis2->setRange(-1.001, 1.001);
        m_samplePlot->xAxis2->setAutoTicks(false);
        m_samplePlot->xAxis2->setAutoTickLabels(false);
        m_samplePlot->xAxis2->setTickVector(x2Ticks);
        m_samplePlot->xAxis2->setTickVectorLabels(x2Label);

        /** actual sample plot **/
        m_samplePlot->addGraph();
        m_samplePlot->graph(0)->setPen(m_leftChannelPen);
        m_samplePlot->graph(0)->setData(xL, yL);
        if( samplePtr->getSampleStructPointer()->sndInfo.channels == stereo )
        {
            m_samplePlot->addGraph();
            m_samplePlot->graph(1)->setPen(m_rightChannelPen);
            m_samplePlot->graph(1)->setData(xR, yR);
        }
        m_samplePlot->replot();

        //refreshLabels();
    }
    else
        resetSamplePlot();
}

void PageSubchannel::resetSamplePlot()
{
#if 1
    qDebug() <<Q_FUNC_INFO;
    m_samplePlot->clearGraphs();
    m_samplePlot->clearItems();
    m_samplePlot->xAxis->setVisible( false);
    m_samplePlot->xAxis2->setVisible(false);
    m_samplePlot->replot();
#endif
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
        qDebug() <<Q_FUNC_INFO  <<"Refresh Step Axis";

        QSharedPointer<Sample> samplePtr = subchannelManager().getSharedPointerToSample();
        int numOfFrames = samplePtr->getSampleStructPointer()->sndInfo.frames;

        QVector<double>  x2Ticks;
        QVector<QString> x2Label;
        int fs = samplePtr->getSampleStructPointer()->sndInfo.samplerate;
        int framesPerStep = (60 * fs) / (4 * subchannelManager().getBpm());
        int steps = 40 * ((numOfFrames) / framesPerStep);

        for( int i = 0; i <= steps; i++ )
        {
            x2Ticks << framesPerStep * i;
            x2Label << QString::number( i );
        }

        m_samplePlot->xAxis2->setVisible(true);
        m_samplePlot->xAxis2->setRange(0, numOfFrames);
        m_samplePlot->yAxis2->setRange(-1.001, 1.001);
        m_samplePlot->xAxis2->setAutoTicks(false);
        m_samplePlot->xAxis2->setAutoTickLabels(false);
        m_samplePlot->xAxis2->setTickVector(x2Ticks);
        m_samplePlot->xAxis2->setTickVectorLabels(x2Label);
        m_samplePlot->replot();
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



void PageSubchannel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption myOption;
    myOption.initFrom(this);
    QPainter myPainter(this);
    style()->drawPrimitive( QStyle::PE_Widget, &myOption, &myPainter,this);
}

