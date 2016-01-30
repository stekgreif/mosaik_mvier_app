#ifndef MYPOT_H
#define MYPOT_H

#include <QWidget>
#include <QDial>

class MyPot : public QDial
{
    Q_OBJECT

    public:
        explicit MyPot(QWidget *parent = 0);

    protected:
        void paintEvent(QPaintEvent *event);
};

#endif // MYPOT_H
