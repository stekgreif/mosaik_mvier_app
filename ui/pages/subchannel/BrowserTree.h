#ifndef BROWSERTREE_H
#define BROWSERTREE_H

#include <QTreeView>

class BrowserTree : public QTreeView
{
    public:
        explicit BrowserTree(QWidget *parent = 0);
        ~BrowserTree();

    void cursorUp(void);
    void cursorDown(void);
    void cursorLeft(void);
    void cursorRight(void);
};

#endif // BROWSERTREE_H
