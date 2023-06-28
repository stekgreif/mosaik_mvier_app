#ifndef DRAWPLAYCURSOR_H
#define DRAWPLAYCURSOR_H

#include <QWidget>



class DrawPlayCursor : public QWidget
{
    Q_OBJECT

    public:
        DrawPlayCursor(QWidget *parent);

    private:
        QWidget *m_background;
        int m_cursorPos;

    public slots:
        void setPlayCursorPosition(int pos);

    protected:
        void paintEvent(QPaintEvent *event);
};

#endif // DRAWPLAYCURSOR_H
