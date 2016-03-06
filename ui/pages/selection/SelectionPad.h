#ifndef SELECTIONPAD_H
#define SELECTIONPAD_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "ui/pages/selection/VolumeWidget.h"
#include <QPixmap>


class SelectionPad : public QWidget
{
    Q_OBJECT

    public:
        explicit SelectionPad(int id, QSize *widgetSize, QWidget *parent = 0);
        ~SelectionPad();

        void setHasSteps(void);
        void clearHasSteps(void);
        void setHasSample(void);
        void clearHasSample(void);
        void setIsPlaying(void);
        void clearIsPlaying(void);
        void setSampleParameters(QString name, float time, float steps);
        void clearSampleName(void);
        void setSampleVolume(void);
        void setIsMute(void);
        void clearIsMute(void);

        void clearPad(void);


    private:
        QSize *m_widgetSize;

        QLabel      *m_buttonPadColor;
        QPushButton *m_buttonPad;
        int          m_padId;
        int          m_padIdRel; //0...3
        QString      m_padOnColor;
        QString      m_padOffColor;

        QLabel *m_hasSample;
        QLabel *m_hasSteps;
        QLabel *m_isPlaying;
        QLabel *m_isTriggert;
        QLabel *m_sampleNameRight;
        QLabel *m_sampleNameLeft;
        QLabel *m_sampleTime;
        QLabel *m_sampleSteps;
        QLabel *m_volume;
        QLabel *m_subChColor;
        QLabel *m_selectionFrame;
        QLabel *m_isMute;

        QLabel *m_sampleWaveForm;
        QPixmap *m_emptySampleWaveForm;

        int wP;
        int hP;

        float m_volumeValue;
        VolumeWidget *m_volumeWidget;

        QLine *m_volBackGround;
        QLine *m_volBar;

    private slots:
        void m_slot_padPressed(void);

    public slots:
        void setPadToSelectionColor(void);
        void setPadToDeselectionColor(void);
        void refreshSampleWaveForm(void);


    signals:
        void signal_subchPadPressed(int padId);




    #if 0 // not implemented yet
    private slots:
    void slot_pressed(void);
    void slot_released(void);

    public slots:
    void slot_selectionUpdate(int id);

    #endif


    protected:
        void paintEvent(QPaintEvent *event);


};

#endif // SELECTIONPAD_H
