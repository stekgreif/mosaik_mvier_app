#ifndef PAGEDEBUG_H
#define PAGEDEBUG_H

#include <QWidget>

class PageDebug : public QWidget
{
    Q_OBJECT

    public:
        explicit PageDebug(QWidget *parent = 0);
        ~PageDebug();

        void refresh(void);

    signals:

    public slots:
};

#endif // PAGEDEBUG_H
