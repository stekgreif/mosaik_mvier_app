#ifndef PAGEINFOS_H
#define PAGEINFOS_H

#include <QWidget>


class PageInfos : public QWidget
{
    Q_OBJECT

    public:
        explicit PageInfos(QWidget *parent = 0);
        ~PageInfos();

        void refresh(void);

    protected:
        void paintEvent(QPaintEvent *event);
};

#endif // PAGEINFOS_H
