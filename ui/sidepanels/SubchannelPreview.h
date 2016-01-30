#ifndef SUBCHANNELPREVIEW_H
#define SUBCHANNELPREVIEW_H

#include <QLabel>
#include <QWidget>
#include <QPainter>
#include <QStyleOption>



class SubchannelPreview : public QWidget
{
    Q_OBJECT

    public:
        explicit SubchannelPreview(QWidget *parent = 0);
        ~SubchannelPreview();
        void refresh(void);

    private:
        //QVector<QLabel> m_box;
        QLabel *m_boxes[64];
        void clearAll(void);
        void setSubchannelSelection(int id);

    protected:
        void paintEvent(QPaintEvent *event);

    public slots:
        void slot_changeSubchannel(int id);

};

#endif // SUBCHANNELPREVIEW_H
