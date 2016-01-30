#ifndef STEPPAD_H
#define STEPPAD_H

#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>


class Pattern;

class StepPad : public QWidget
{
    Q_OBJECT

    public:
        StepPad(int id, QWidget *parent);
        ~StepPad();

        void setStep(void);
        void clrStep(void);
        void tglStep(void);
        void setStepVal(bool val);

        void setSeq(void);
        void clrSeq(void);
        void tglSeq(void);
        void setSeqVal(bool val);

        void setSize(int x, int y);


    signals:
        void signal_padPressed(int id);

    private slots:
        void slot_btnPressed(void);

    protected:
        void paintEvent(QPaintEvent *event);

    private:
        int m_id;
        QPushButton *m_buttonPad;
        QWidget     *m_colorPad;
};

#endif // STEPPAD_H
