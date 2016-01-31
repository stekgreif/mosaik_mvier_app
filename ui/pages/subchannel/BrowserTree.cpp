#include "BrowserTree.h"

#include <QKeyEvent>

BrowserTree::BrowserTree(QWidget *parent)
    : QTreeView(parent)
{

}

BrowserTree::~BrowserTree()
{

}



void BrowserTree::cursorUp()
{
    QKeyEvent *event = nullptr;
    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier, "up", false, 0);
    keyPressEvent(event);
}

void BrowserTree::cursorDown()
{
    QKeyEvent *event = nullptr;
    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier, "down", false, 0);
    keyPressEvent(event);
}

void BrowserTree::cursorLeft()
{
    QKeyEvent *event = nullptr;
    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier, "left", false, 0);
    keyPressEvent(event);
}

void BrowserTree::cursorRight()
{
    QKeyEvent *event = nullptr;
    event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier, "right", false, 0);
    keyPressEvent(event);
}
