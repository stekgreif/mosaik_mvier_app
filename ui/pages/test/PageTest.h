#ifndef PAGETEST_H
#define PAGETEST_H

#include <QWidget>


class PageTest : public QWidget
{
    Q_OBJECT

    public:
        explicit PageTest(QWidget *parent = 0);
        ~PageTest();

    protected:
        void paintEvent(QPaintEvent *event);
};

#endif // PAGETEST_H
