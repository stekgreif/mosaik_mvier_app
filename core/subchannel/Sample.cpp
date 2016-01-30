#include "Sample.h"
#include <QDebug>



Sample::Sample(QString samplePathAndName)
{
    m_sample = new Sample_t;

    m_sample->path = samplePathAndName.section("/", 0,-2);
    qDebug() <<Q_FUNC_INFO <<m_sample->path;

    QString nameWithExt = samplePathAndName.section("/",-1,-1);     // file.wav
    m_sample->name = nameWithExt.section(".", -2, -2);              // file
    qDebug() <<Q_FUNC_INFO <<"File Name:" <<m_sample->name;

    m_sample->pathAndName = samplePathAndName;
    m_sample->sndFile     = sf_open(m_sample->pathAndName.toUtf8().constData(), SFM_READ, &m_sample->sndInfo);
    m_sample->lengthInMs  = ((float) m_sample->sndInfo.frames * 1000) / m_sample->sndInfo.samplerate;

#if 1 // 2015-05-12 working version
    m_sample->frameBuffer = (float*) malloc( m_sample->sndInfo.frames * m_sample->sndInfo.channels * sizeof(float));
    if(sf_readf_float(m_sample->sndFile, m_sample->frameBuffer, m_sample->sndInfo.frames) == 0)
        qDebug() <<Q_FUNC_INFO <<"Could not read Sample.";
#endif

#if 0 // 2015-05-12 untested viewFrameBuffer
    m_sample->viewFrameBuffer = (float*) malloc( m_sample->sndInfo.frames * m_sample->sndInfo.channels * sizeof(float));
    for( int i = 0; i < m_sample->sndInfo.frames; i++ )
    {
        m_sample->viewFrameBuffer[i] = m_sample->frameBuffer[i] + m_sample->frameBuffer[i+1];
        i++;
    }
#endif

#if 0 //2015-05-12 new stereo version -> untested
    float *tempBuffer;
    if( m_sample->sndInfo.frames > 0)
    {
        if( m_sample->sndInfo.channels == 1 )
        {
            // mono
            tempBuffer = (float*) malloc( m_sample->sndInfo.* m_sample->sndInfo.channels * sizeof(float));
            if(sf_readf_float(m_sample->sndFile, tempBuffer, m_sample->sndInfo.frames) == 0)
                qDebug() <<Q_FUNC_INFO <<"Could not read Sample.";
        }
        else if ( m_sample->sndInfo.channels == 2 )
        {
            qDebug() <<Q_FUNC_INFO <<"load to stereo";
        }
        else
            qDebug() <<Q_FUNC_INFO <<"multichannel not supported";
    }
#endif


#if 0
    qDebug() <<"channels:"      <<m_sample->info.channels;
    qDebug() <<"frames:"        <<m_sample->info.frames;
    qDebug() <<"path + name:"   <<m_sample->pathAndName;
    qDebug() <<"sample rate:"   <<m_sample->info.samplerate;
    qDebug() <<"length in ms:"  <<m_sample->lengthInMs;
    qDebug() <<"length in sec:" <<m_sample->lengthInMs / 1000;
#endif
    qDebug() <<Q_FUNC_INFO;
}



Sample::~Sample()
{
    if( sf_close(m_sample->sndFile) == 0 )
        qDebug() <<Q_FUNC_INFO <<"Sample closed.";
    else
        qDebug() <<Q_FUNC_INFO <<"Problem closing sample.";

    delete m_sample;
}



Sample_t *Sample::getSampleStructPointer(void)
{
    return m_sample;
}




