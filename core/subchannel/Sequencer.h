#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <QList>

class Sequencer
{
public:
    Sequencer();
    ~Sequencer();

    void setStep(int id);
    void clearStep(int id);
    int  getStepValue(int id);

    //QList getSetSteps(void);

private:
    int m_stepCounter;
    int m_pattern[64];
};

#endif // SEQUENCER_H
