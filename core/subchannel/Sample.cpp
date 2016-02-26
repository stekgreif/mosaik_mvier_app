#include "Sample.h"
#include <QDebug>
#include <QPainter>


Sample::Sample(QString samplePathAndName)
{
    m_sample = new Sample_t;

    m_sample->path = samplePathAndName.section("/", 0,-2);
    //qDebug() <<Q_FUNC_INFO <<m_sample->path;

    QString nameWithExt = samplePathAndName.section("/",-1,-1);     // file.wav
    m_sample->name = nameWithExt.section(".", -2, -2);              // file
    //qDebug() <<Q_FUNC_INFO <<"File Name:" <<m_sample->name;

    m_sample->pathAndName = samplePathAndName;
    m_sample->sndFile     = sf_open(m_sample->pathAndName.toUtf8().constData(), SFM_READ, &m_sample->sndInfo);
    m_sample->lengthInMs  = ((float) m_sample->sndInfo.frames * 1000) / m_sample->sndInfo.samplerate;

    m_sample->frameBuffer = (float*) malloc( m_sample->sndInfo.frames * m_sample->sndInfo.channels * sizeof(float));
    if(sf_readf_float(m_sample->sndFile, m_sample->frameBuffer, m_sample->sndInfo.frames) == 0)
    {
        qDebug() <<Q_FUNC_INFO <<"Could not read Sample.";
    }


    qDebug() <<"frames:" <<m_sample->sndInfo.frames;

#if 0
    qDebug() <<"channels:"      <<m_sample->info.channels;
    qDebug() <<"frames:"        <<m_sample->info.frames;
    qDebug() <<"path + name:"   <<m_sample->pathAndName;
    qDebug() <<"sample rate:"   <<m_sample->info.samplerate;
    qDebug() <<"length in ms:"  <<m_sample->lengthInMs;
    qDebug() <<"length in sec:" <<m_sample->lengthInMs / 1000;
    qDebug() <<Q_FUNC_INFO;
#endif

    QSize waveFormSize(780,300);
    m_sample->pixmap = new QPixmap(waveFormSize);
    m_sample->pixmap->fill(Qt::white);

    QPainter painter(m_sample->pixmap);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::blue);

    /** only mono **/
    int stepWidth = m_sample->sndInfo.frames / waveFormSize.width();
    qDebug() <<Q_FUNC_INFO <<"step Width:" <<stepWidth;


    QLine linePos(1,2,3,4);
    QLine lineNeg(1,2,3,4);

    linePos.setLine(0, 150, 780, 150);
    painter.drawLine(linePos);
    painter.setPen(pen);


    float maxVal = 0;
    float minVal = 0;
    int stepCnt = 0;
    float temp = 0;

    for(int frame = 0; frame < m_sample->sndInfo.frames; frame = frame + stepWidth)
    {
        maxVal = 0;
        minVal = 0;

        for(int cnt = 0; cnt < stepWidth; cnt++)
        {
            temp = m_sample->frameBuffer[frame + cnt];

            if( temp < 0.0 )            //negative
            {
                if( temp < minVal )
                {
                    minVal = temp;
                }
            }
            else                        // positive
            {
                if( temp > maxVal )
                {
                    maxVal = temp;
                }
            }
        }

        //qDebug() <<"xxxxxxxxxxxxxxxxx pos" <<maxVal;
        //qDebug() <<"xxxxxxxxxxxxxxxxx neg" <<minVal;

        linePos.setLine(stepCnt, 150, stepCnt, 150 - (maxVal*150) );
        lineNeg.setLine(stepCnt, 150, stepCnt, 150 - (minVal*150) );
        painter.drawLine(linePos);
        painter.drawLine(lineNeg);

        stepCnt++;
    }

    qDebug() <<Q_FUNC_INFO <<"max abs value:" <<getMaxSampleValue()  <<"Sample loaded:" <<m_sample->pathAndName;
}



Sample::~Sample()
{
    if( sf_close(m_sample->sndFile) == 0 )
    {
        qDebug() <<Q_FUNC_INFO <<"Closing Sample:" <<m_sample->pathAndName;
    }
    else
    {
        qDebug() <<Q_FUNC_INFO <<"Problem closing sample.";
    }

    delete m_sample;
}



Sample_t *Sample::getSampleStructPointer(void)
{
    return m_sample;
}



float Sample::getMaxSampleValue()
{
    float max = 0.0;
    float temp = 0.0;

    for(int frame = 0; m_sample->sndInfo.frames > frame; frame++)
    {
        temp = m_sample->frameBuffer[frame];

        if( temp < 0.0 )
            temp = -temp;

        if( max < temp )
            max = temp;
    }

    //qDebug() <<"yyyyyyy  max abs:" <<max;
    return max;
}

float Sample::getMaxSumValue()
{
    int stepCnt = 0;
    float sum = 0;
    float max = 0;
    float temp = 0;
    int stepWidth = m_sample->sndInfo.frames / 780;

    for(int frame = 0; frame < m_sample->sndInfo.frames; frame = frame + stepWidth)
    {
        sum = 0;

        for(int cnt = 0; cnt < stepWidth; cnt++)
        {
            if( temp < 0.0 )
                temp = -temp;

            sum = sum + temp;
            sum = sum / (float)stepWidth;
        }

        if( sum < 0.0 )
            sum = -sum;

        if( max < sum )
            max = sum;

        stepCnt++;
    }

    return max;
}




