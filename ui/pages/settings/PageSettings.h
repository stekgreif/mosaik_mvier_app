#ifndef PAGESETTINGS_H
#define PAGESETTINGS_H

#include <QWidget>


class PageSettings : public QWidget
{
    Q_OBJECT

    public:
        explicit PageSettings(QWidget *parent = 0);
        ~PageSettings();

        void refresh(void);

    protected:
        void paintEvent(QPaintEvent *event);
};

#endif // PAGESETTINGS_H
