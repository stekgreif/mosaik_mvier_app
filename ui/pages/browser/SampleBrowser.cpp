#include "SampleBrowser.h"
#include <QKeyEvent>

SampleBrowser::SampleBrowser(QWidget *parent)
    : QTreeView(parent)
{

}

SampleBrowser::~SampleBrowser()
{

}



void SampleBrowser::cursorUp()
{
    QKeyEvent *event = nullptr;
    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier, "up", false, 0);
    keyPressEvent(event);
}

void SampleBrowser::cursorDown()
{
    QKeyEvent *event = nullptr;
    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier, "down", false, 0);
    keyPressEvent(event);
}

void SampleBrowser::cursorLeft()
{
    QKeyEvent *event = nullptr;
    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier, "left", false, 0);
    keyPressEvent(event);
}

void SampleBrowser::cursorRight()
{
    QKeyEvent *event = nullptr;
    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier, "right", false, 0);
    keyPressEvent(event);
}

