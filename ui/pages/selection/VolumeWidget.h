#ifndef VOLUMEWIDGET_H
#define VOLUMEWIDGET_H

#include <QObject>
#include <QWidget>


class VolumeWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit VolumeWidget(QWidget *parent = 0);
        ~VolumeWidget();

        void setVolume(float vol); // 0.0 .. 1.0

    protected:
        void paintEvent(QPaintEvent *event);

    private:
        float m_value;
};

#endif // VOLUMEWIDGET_H
