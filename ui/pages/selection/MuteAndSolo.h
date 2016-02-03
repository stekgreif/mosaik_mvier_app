#ifndef MUTEANDSOLO_H
#define MUTEANDSOLO_H

#include <QWidget>
#include "Settings.h"
#include "ui/pages/selection/MuteAndSoloPad.h"


class MuteAndSolo : public QWidget
{
    Q_OBJECT

    public:
        explicit MuteAndSolo(QWidget *parent);
        ~MuteAndSolo();
        void display(bool state);
        void refresh();

    private:
        MuteAndSoloPad *m_muteAndSoloPad[SETTINGS_NUM_OF_SUBS];

    private slots:
        void slot_stateChanged(int id);

    signals:
        void signal_mutePadPressed(int id);

    protected:
        void paintEvent(QPaintEvent *event);
};

#endif // MUTEANDSOLO_H
