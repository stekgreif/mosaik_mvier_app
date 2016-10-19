#include "Envelope.h"
#include <QDebug>

Envelope::Envelope()
{
    resetAll();
}

Envelope::~Envelope()
{

}



envelope_t Envelope::getEnvelope()
{
    return m_envelope;
}

void Envelope::setStartPointRel(float start)
{
    float startAbs = start + m_envelope.start;
    if( startAbs < 0 )
        startAbs = 0;
    float fInAbs   = start + m_envelope.fadeIn;
    if( fInAbs < 0 )
        fInAbs = 0;

	if( (startAbs >= 0) && (startAbs <= m_envelope.fadeIn+0.01) && (fInAbs <= m_envelope.fadeOut)  )
    {
        m_envelope.start  = startAbs;
        m_envelope.fadeIn = fInAbs;
        qDebug() <<Q_FUNC_INFO <<"start point changed to:" <<startAbs;
    }
    else
        qDebug() <<Q_FUNC_INFO <<"Not in range: start" <<start <<"fadeIn:" <<m_envelope.fadeIn;
}


void Envelope::setFadeInPointRel(float fadeIn)
{
    float fInAbs  = fadeIn + m_envelope.fadeIn;

    if( (fInAbs >= m_envelope.start) && (fInAbs <= m_envelope.fadeOut) )
    {
        m_envelope.fadeIn = fInAbs;
        qDebug() <<Q_FUNC_INFO <<"fade in point changed to:" <<fInAbs;
    }
    else
        qDebug() <<Q_FUNC_INFO <<"fade in point not in range" <<fadeIn;
}


void Envelope::setFadeOutPointRel(float fadeOut)
{
    float fOutAbs = fadeOut + m_envelope.fadeOut;

	if( (fOutAbs >= m_envelope.fadeIn) && (fOutAbs <= m_envelope.end) )
    {
        m_envelope.fadeOut = fOutAbs;
        qDebug() <<Q_FUNC_INFO <<"fade out point changed to:" <<fOutAbs;
    }
    else
        qDebug() <<Q_FUNC_INFO <<"fade out point not in range" <<fadeOut;
}


void Envelope::setEndPointRel(float end)
{
    float endAbs  = end + m_envelope.end;
    float fOutAbs = end + m_envelope.fadeOut;

	if( (endAbs >= m_envelope.fadeOut-0.01) && (endAbs <= 1) && (fOutAbs >= m_envelope.fadeIn) )
    {
        m_envelope.end     = endAbs;
        m_envelope.fadeOut = fOutAbs;
        qDebug() <<Q_FUNC_INFO <<"end point changed to:" <<endAbs;
    }
    else
        qDebug() <<Q_FUNC_INFO <<"end point not in range" <<end;
}




void Envelope::setStartOffsetPoint(float offset)
{
    Q_UNUSED(offset)
    qDebug() <<Q_FUNC_INFO <<"offset not implemented yet";
}



void Envelope::setRelativeStart(float start)
{
    start = start / 100;

    if( ((m_envelope.start + start ) >= 0) && ((m_envelope.start + start <= 1) ))
    {
        m_envelope.start = m_envelope.start + start;
        qDebug() <<Q_FUNC_INFO <<"start point changed to:" <<m_envelope.start;
    }
    else
        qDebug() <<Q_FUNC_INFO <<"start point not in range";
}

void Envelope::setRelativeEnd(float end)
{
    Q_UNUSED(end);
}

void Envelope::setRelativeFadeIn(float fadeIn)
{
    Q_UNUSED(fadeIn);
}

void Envelope::setRelativeFadeOut(float fadeOut)
{
    Q_UNUSED(fadeOut);
}






void Envelope::resetAll()
{
    m_envelope.start = 0.0;
    m_envelope.end = 1.0;
    m_envelope.fadeIn = 0.0;
    m_envelope.fadeOut = 1.0;
}
