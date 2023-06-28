#ifndef DRAWENVELOPE_H
#define DRAWENVELOPE_H

#include <QWidget>
#include <QObject>
#include "MosaikTypes.h"


class DrawEnvelope : public QWidget
{
    Q_OBJECT

    public:
        DrawEnvelope(QSize size, QWidget *parent);
        void setEnvelope(envelope_t envelope);

    private:
        envelope_t *m_envelope;
        QSize      *m_widgetSize;
        void refresh(void);

    protected:
        void paintEvent(QPaintEvent *event);
};
#endif // DRAWENVELOPE_H
