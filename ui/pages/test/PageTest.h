#ifndef PAGETEST_H
#define PAGETEST_H

#include <QWidget>
#include <QScrollArea>
#include <QLabel>


class PageTest : public QWidget
{
    Q_OBJECT

    public:
        explicit PageTest(QWidget *parent = 0);
        ~PageTest();

        void refresh(void);

    protected:
        void paintEvent(QPaintEvent *event);
        QScrollArea *m_scrollArea;
        QLabel *m_toolbarDummy;
};

#endif // PAGETEST_H
