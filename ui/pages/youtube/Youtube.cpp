#include "Youtube.h"
#include <QProcess>


Youtube::Youtube(QWidget *parent)
    : QWidget(parent)
{
    m_browser = new QWebView(this);


    m_browser->move(20,20);
    /// @todo 43 - get size from settings
    m_browser->setFixedSize(1080,1000);
    m_browser->setUrl(QUrl("http://www.youtube.com"));
    m_browser->show();
    m_browser->update();


    /* virtual keyboard */
    m_keyboardProcess = new QProcess;

    m_showKeyboard = new QPushButton(this);
    m_showKeyboard->setFixedSize(240,40);
    m_showKeyboard->move(200,1030);
    m_showKeyboard->setText("Show Keyboard");

    m_hideKeyboard = new QPushButton(this);
    m_hideKeyboard->setFixedSize(240,40);
    m_hideKeyboard->move(600,1030);
    m_hideKeyboard->setText("Hide Keyboard");

    connect(m_showKeyboard, SIGNAL(pressed()), this, SLOT(slot_showKeyboard()) );
    connect(m_hideKeyboard, SIGNAL(pressed()), this, SLOT(slot_hideKeyboard()) );

}




Youtube::~Youtube()
{

}

void Youtube::refresh()
{

}


void Youtube::slot_showKeyboard()
{
    m_keyboardProcess->start("onboard");
}


void Youtube::slot_hideKeyboard()
{
    m_keyboardProcess->kill();
}

