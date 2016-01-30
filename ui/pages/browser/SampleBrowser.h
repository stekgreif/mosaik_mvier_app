#ifndef SAMPLEBROWSER_H
#define SAMPLEBROWSER_H

#include <QTreeView>

class SampleBrowser : public QTreeView
{
    public:
        explicit SampleBrowser(QWidget *parent = 0);
        ~SampleBrowser();

        void cursorUp(void);
        void cursorDown(void);
        void cursorLeft(void);
        void cursorRight(void);
};

#endif // SAMPLEBROWSER_H
