#ifndef PATTERN_H
#define PATTERN_H

#include <QWidget>
//#include <QPushButton>
#include "ui/pages/subchannel/StepPad.h"

class MosaikMiniApp;

class Pattern : public QWidget
{
    Q_OBJECT

    public:
        explicit Pattern(MosaikMiniApp *mosaikMiniApp = 0, QWidget *parent = 0);
        ~Pattern();

        void setStep(int id);
        void clrStep(int id);

        void refresh(void);

    private slots:
        void slot_padPressed(int id);

    signals:
        void signal_padPressed(int id);

    private:
        StepPad *m_button[64];
        MosaikMiniApp *m_mosaikMiniApp;
};

#endif // PATTERN_H
