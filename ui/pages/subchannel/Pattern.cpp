#include "Pattern.h"
#include <QDebug>
#include <QLabel>
#include "core/subchannel/SubchannelManager.h"
#include "MosaikMiniApp.h"



Pattern::Pattern(MosaikMiniApp *mosaikMiniApp, QWidget *parent)
    : QWidget(parent)
    , m_mosaikMiniApp(mosaikMiniApp)
{
    qDebug() <<Q_FUNC_INFO <<"Init";


#if 0 // original button sizes
    const int btnSize   = 34;
    const int btnOffset =  4;
    const int rowOffset = 16 * (btnSize + btnOffset);
#endif

#if 1
    const int btnSize   = 7;
    const int btnOffset = 1;
    const int rowOffset = 16 * (btnSize + btnOffset);
#endif

    for (int i = 0; i < 64; i++)
    {
        m_button[i] = new StepPad(i, this);
        m_button[i]->setFixedSize(btnSize, btnSize);
        m_button[i]->setSize(btnSize, btnSize);
        connect(m_button[i], SIGNAL(signal_padPressed(int)), this, SLOT(slot_padPressed(int)) );
    }

    for (int i = 0; i < 16; i++)
        m_button[i]->move(i * (btnSize + btnOffset), btnOffset);

    for (int i = 16; i < 32; i++)
        m_button[i]->move(rowOffset, btnOffset + (i-16) * (btnSize + btnOffset));

    for (int i = 32; i < 48; i++)
        m_button[i]->move(rowOffset - (i-32) * (btnSize + btnOffset), rowOffset + btnOffset);

    for (int i = 48; i < 64; i++)
        m_button[i]->move(0, rowOffset - (i-48) * (btnSize + btnOffset) + btnOffset);
}


Pattern::~Pattern()
{

}

void Pattern::setStep(int id)
{
    m_button[id]->setStep();
}

void Pattern::clrStep(int id)
{
    m_button[id]->clrStep();
}



void Pattern::refresh()
{
    QBitArray pattern = subchannelManager().getCurrentSubchannelPattern();

    for (int i = 0; i < 64; i++)
    {
        if( pattern.at(i) == 1)
            setStep(i);
        else
            clrStep(i);
    }

    //qDebug() <<Q_FUNC_INFO <<pattern;
}



void Pattern::slot_padPressed(int id)
{
    //qDebug() <<Q_FUNC_INFO <<"pad pressed";
    emit signal_padPressed(id);
}

