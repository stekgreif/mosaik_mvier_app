#ifndef ROTARY_H
#define ROTARY_H

#include <QWidget>
#include <QLabel>
#include <QDial>
#include "mypot.h"

class Rotary : public QWidget
{
    Q_OBJECT

public:
    explicit Rotary(int id, QWidget *parent = 0);
    
private:
    //QDial  *m_poti;
    int     m_id;
    QLabel *m_labelName;
    QLabel *m_labelValue;
    MyPot  *m_poti;
    bool    m_isSelected;
    void valChanged(int val);
    
public slots:
    void slot_setSelection(bool state);
    void slot_setName(QString name);
    void slot_setValue(double value);

private slots:
    void slot_potValChanged(int);

signals:
    void signal_valueChanged(int id, float value);

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // ROTARY_H
