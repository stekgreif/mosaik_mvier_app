#include "SelectionPad.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>
#include "core/subchannel/SubchannelManager.h"


SelectionPad::SelectionPad(int id, QSize *widgetSize, QWidget *parent)
    : QWidget(parent)
{
    m_padWidgetId = id;

    //qDebug() <<Q_FUNC_INFO <<"InitPad:" <<m_padWidgetId;


    m_widgetSize = new QSize;
    m_widgetSize->setHeight(widgetSize->height());
    m_widgetSize->setWidth(widgetSize->width());

    int wP = m_widgetSize->width()/20;      // 5 per step
    int hP = m_widgetSize->height()/20;
    int mS = 2;
    int lS = 12;



    /**  layer 0 - widget **/
    this->setObjectName("pad_widget");
    this->setFixedSize(*m_widgetSize);


    m_volumeWidget = new VolumeWidget(this);
    m_volumeWidget->move(wP*20.2, hP*6.7);


    /** layer 1 - info tiles **/
    m_sampleNameRight = new QLabel(this);
    m_sampleNameRight->setObjectName("sampleNameRight");
    m_sampleNameRight->setStyleSheet("QLabel#sampleNameRight {background-color: rgba(127,127,127, 0%); color: rgb(180,180,180); }");
    m_sampleNameRight->move( (m_widgetSize->width()/20)*2, (m_widgetSize->height()/5) * 2.0 );
    m_sampleNameRight->setAlignment(Qt::AlignRight);
    m_sampleNameRight->setFixedWidth((m_widgetSize->width()/20)*18);

    m_sampleNameLeft = new QLabel(this);
    m_sampleNameLeft->setObjectName("sampleNameLeft");
    m_sampleNameLeft->setStyleSheet("QLabel#sampleNameLeft {background-color: rgba(127,127,127, 0%); color: rgb(180,180,180); }");
    m_sampleNameLeft->move( (m_widgetSize->width()/20)*2, (m_widgetSize->height()/5) * 1.4 );
    m_sampleNameLeft->setAlignment(Qt::AlignLeft);
    m_sampleNameLeft->setFixedWidth((m_widgetSize->width()/20)*18);

    m_sampleTime = new QLabel(this);
    m_sampleTime->setObjectName("sampleTime");
    m_sampleTime->setStyleSheet("QLabel#sampleTime {background-color: rgba(127,127,127, 0%); color: rgb(180,180,180); }");
    m_sampleTime->move( (m_widgetSize->width()/20)*2, (m_widgetSize->height()/5) * 4 );
    m_sampleTime->setAlignment(Qt::AlignCenter);

    m_sampleSteps = new QLabel(this);
    m_sampleSteps->setObjectName("sampleSteps");
    m_sampleSteps->setStyleSheet("QLabel#sampleSteps {background-color: rgba(127,127,127, 0%); color: rgb(180,180,180); }");
    m_sampleSteps->move( ((m_widgetSize->width()/20) * 14), (m_widgetSize->height()/5) * 4 );
    m_sampleSteps->setAlignment(Qt::AlignCenter);

#if 0 // replaced by SampleSteps
    m_volume = new QLabel(this);
    m_volume->setObjectName("volume");
    m_volume->setStyleSheet("QLabel#volume {background-color: rgba(127,127,127, 0%); color: rgb(180,180,180); }");
    m_volume->move( ((m_widgetSize->width()/20) * 16), (m_widgetSize->height()/5) * 4 );
    m_volume->setAlignment(Qt::AlignRight);
#endif

#if 1 // subchannel color pads
    m_subChColor = new QLabel(this);
    m_subChColor->setObjectName("color");
    switch (id%4)
    {
        case 0:  //red 255,0,0
            m_subChColor->setStyleSheet("QLabel#color {background-color: rgba(255,0,0,15%);}");
            break;
        case 1: //blue 0,0,255
            m_subChColor->setStyleSheet("QLabel#color {background-color: rgba(0,0,255,15%);}");
            break;
        case 2: //yellow 255,255,0
            m_subChColor->setStyleSheet("QLabel#color {background-color: rgba(255,255,0,15%);}");
            break;
        case 3: //magenta 255,0,255
            m_subChColor->setStyleSheet("QLabel#color {background-color: rgba(255,0,255,15%);}");
            break;
        default:
            break;
    }
    m_subChColor->setFixedSize(14,14);
    m_subChColor->move( (m_widgetSize->width()/20)*9.9 , m_widgetSize->height()/5 * 4);
#endif

    /** tiles **/
    m_isTriggert = new QLabel(this);
    m_isTriggert->setObjectName("tile_isTriggert");
    m_isTriggert->setFixedSize(wP*8, hP*4);
    m_isTriggert->move( wP*mS, hP*mS );
    m_isTriggert->setText("TRG");
    m_isTriggert->setAlignment(Qt::AlignCenter);

    m_hasSample = new QLabel(this);
    m_hasSample->setObjectName("tile_hasSample");
    m_hasSample->setStyleSheet("QLabel#tile_hasSample {background-color: rgb(127,127,127);}");
    m_hasSample->setText("SAMPL");
    m_hasSample->setAlignment(Qt::AlignCenter);
    m_hasSample->setFixedSize( wP*8, hP*4);
    m_hasSample->move( wP*lS, hP*lS );

    m_hasSteps = new QLabel(this);
    m_hasSteps->setObjectName("tile_hasStep");
    m_hasSteps->setStyleSheet("QLabel#tile_hasStep {background-color: rgb(127,127,127);}");
    m_hasSteps->setFixedSize( wP*8, hP*4 );
    m_hasSteps->move( wP*mS, hP*lS  );
    m_hasSteps->setText("STEP");
    m_hasSteps->setAlignment(Qt::AlignCenter);

    m_isPlaying = new QLabel(this);
    m_isPlaying->setObjectName( "tile_isPlaying");
    m_isPlaying->setFixedSize( wP*8, hP*4);
    m_isPlaying->move( wP*lS, hP*mS);
    m_isPlaying->setText("PLAY");
    m_isPlaying->setAlignment(Qt::AlignCenter);


    /** layer 2 - button color */
    m_buttonPadColor = new QLabel(this);
    m_buttonPadColor->setObjectName("pad_buttonColor");
    m_buttonPadColor->setFixedSize(*m_widgetSize);


    /** layer 3 - button **/
    m_buttonPad = new QPushButton(this);
    m_buttonPad->setObjectName("pad_button");
    m_buttonPad->setFixedSize(*m_widgetSize);
    //m_buttonPad->setText(QString::number(id));



    /* internal connections */
    connect(m_buttonPad, SIGNAL(pressed()),  this, SLOT(m_slot_padPressed()) );
}

SelectionPad::~SelectionPad()
{

}



/** tiles **/
void SelectionPad::setHasSteps()
{
    //m_hasSteps->setStyleSheet("QLabel#tile_hasStep {background-color: rgb(127,255,127);}");
    m_hasSteps->setStyleSheet("QLabel#tile_hasStep {background-color: rgb(250,250,250);}");
}

void SelectionPad::clearHasSteps()
{
    m_hasSteps->setStyleSheet("QLabel#tile_hasStep {background-color: rgb(127,127,127);}");
}

void SelectionPad::setHasSample()
{
    //m_hasSample->setStyleSheet("QLabel#tile_hasSample {background-color: rgb(180,180,215);}");
    m_hasSample->setStyleSheet("QLabel#tile_hasSample {background-color: rgb(250,250,250);}");
}

void SelectionPad::clearHasSample()
{
    m_hasSample->setStyleSheet("QLabel#tile_hasSample {background-color: rgb(127,127,127);}");
}

void SelectionPad::setIsPlaying()
{
    //m_isPlaying->setStyleSheet("QLabel#tile_isPlaying {background-color: rgb(255,255,127);}");
    m_isPlaying->setStyleSheet("QLabel#tile_isPlaying {background-color: rgb(250,250,250);}");
}

void SelectionPad::clearIsPlaying()
{
    m_isPlaying->setStyleSheet("QLabel#tile_isPlaying {background-color: rgb(127,127,127);}");
}


void SelectionPad::setSampleParameters(QString name, float time, float steps)
{
    m_sampleNameRight->setText(name);
    m_sampleNameRight->adjustSize();
    m_sampleNameLeft->setText(name);
    m_sampleNameLeft->adjustSize();
    m_sampleTime->setText( QString::number(time/1000, 'f', 2) + " s" );
    m_sampleTime->adjustSize();
    m_sampleSteps->setText( QString::number(steps, 'f', 2) );
    m_sampleSteps->adjustSize();
}

void SelectionPad::clearSampleName()
{
    m_sampleNameRight->setText(" ");
    m_sampleTime->setText(" ");
}

void SelectionPad::setSampleVolume()
{
    //qDebug() <<Q_FUNC_INFO <<m_volumeValue;

    m_volumeValue = subchannelManager().getSubchannelVolume(m_padWidgetId);
    //m_volume->setText( QString::number( m_volumeValue, 'f', 2 ) );
    //m_volume->adjustSize();
    m_volumeWidget->setVolume(m_volumeValue);
}


void SelectionPad::m_slot_padPressed()
{
    emit signal_subchPadPressed(m_padWidgetId);
}

void SelectionPad::setPadToSelectionColor(void)
{
    switch (m_padWidgetId%4)
    {
        case 0:  //red 255,0,0
            m_subChColor->setStyleSheet("QLabel#color {background-color: rgba(255,0,0,100%);}");
            break;
        case 1: //blue 0,0,255
            m_subChColor->setStyleSheet("QLabel#color {background-color: rgba(0,0,255,100%);}");
            break;
        case 2: //yellow 255,255,0
            m_subChColor->setStyleSheet("QLabel#color {background-color: rgba(255,255,0,100%);}");
            break;
        case 3: //magenta 255,0,255
            m_subChColor->setStyleSheet("QLabel#color {background-color: rgba(255,0,255,100%);}");
            break;
        default:
            break;
    }
}



void SelectionPad::setPadToDeselectionColor(void)
{
    switch (m_padWidgetId%4)
    {
        case 0:  //red 255,0,0
            m_subChColor->setStyleSheet("QLabel#color {background-color: rgba(255,0,0,15%);}");
            break;
        case 1: //blue 0,0,255
            m_subChColor->setStyleSheet("QLabel#color {background-color: rgba(0,0,255,15%);}");
            break;
        case 2: //yellow 255,255,0
            m_subChColor->setStyleSheet("QLabel#color {background-color: rgba(255,255,0,15%);}");
            break;
        case 3: //magenta 255,0,255
            m_subChColor->setStyleSheet("QLabel#color {background-color: rgba(255,0,255,15%);}");
            break;
        default:
            break;
    }
}



void SelectionPad::paintEvent(QPaintEvent *event)
{
    //qDebug() <<Q_FUNC_INFO;

    Q_UNUSED(event);
    QStyleOption myOption;
    myOption.initFrom(this);
    QPainter myPainter(this);
    style()->drawPrimitive( QStyle::PE_Widget, &myOption, &myPainter, this);
}
