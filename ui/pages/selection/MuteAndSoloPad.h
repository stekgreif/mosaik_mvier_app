#ifndef MUTEANDSOLOPAD_H
#define MUTEANDSOLOPAD_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>


class MuteAndSoloPad : public QWidget
{
    Q_OBJECT

    public:
        explicit MuteAndSoloPad(int id, QSize *widgetSize, QWidget *parent);
        ~MuteAndSoloPad();

    signals:
        void signal_mutePadPressed(int id);

    public slots:
        void slot_setPadToMuteColor(void);
        void slot_setPadToUnmuteColor(void);

    private:
        int          m_id;
        QSize       *m_widgetSize;
        QLabel      *m_buttonPadColor;
        QPushButton *m_buttonPad;

    private slots:
        void slot_padPressed(void);

    protected:
        void paintEvent(QPaintEvent *event);
};

#endif // MUTEANDSOLOPAD_H
