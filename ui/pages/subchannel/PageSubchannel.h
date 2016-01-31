#ifndef PAGESUBCHANNEL_H
#define PAGESUBCHANNEL_H

#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>

#include </usr/include/sndfile.h>

#include "ui/pages/subchannel/qcustomplot.h"
#include <ui/pages/subchannel/rotary.h>
#include <ui/pages/subchannel/DrawEnvelope.h>
#include <ui/pages/subchannel/Pattern.h>

#include "ui/pages/browser/PageBrowser.h"


class UiManager;
class MosaikMiniApp;

class PageSubchannel : public QWidget
{
    Q_OBJECT

    public:
        explicit PageSubchannel(MosaikMiniApp *mosaikMiniApp = 0, UiManager *parent = 0);
        //explicit PageSubchannel(UiManager *parent, QWidget *uiParent = 0);
        ~PageSubchannel();
        void refresh(void);
        void refreshEnvelope(void);
        void refreshPots(void);
        void refreshLabels(void);
        void refreshStepAxis(void);

    public slots:
        void slot_changePathId(int pathId);

    private slots:
        void slot_potPosChanged(int id, float value);
        void slot_changePage(void);

    protected:
        void paintEvent(QPaintEvent *event);

    private:
        /** order of appearance can be changed here */
        struct SampleInfoIds
        {
            enum sampleInfoIds
            {
                samplerate,
                channels,
                frames,
                length,
                start,
                end,
                delta,
                steps,
                volume,
                pan
            };
        };
        QVector<QLabel*> *m_samplePropertyNameLabels;
        QVector<QLabel*> *m_samplePropertyLabel;
        QVector<QString> *m_samplePropertyNames;

        enum AudioChannels{
            mono = 1,
            stereo = 2
        };

        QPen m_leftChannelPen;
        QPen m_rightChannelPen;

        UiManager *m_parent;
        MosaikMiniApp *m_mosaikMiniApp;



        /** path and file system **/
        QFileSystemModel *m_fileSystem;
        SampleBrowser    *m_treeView;
        QString m_path[4];
        QString m_pathName[4];



        /** sample view **/
        QCustomPlot *m_samplePlot;
        void refreshSamplePlot(void);
        void resetSamplePlot(void);

        QLabel *m_headline;
        QLabel *m_labelVarPathAndName;

        QPushButton *m_changePage;


        DrawEnvelope *m_envelope;
        Pattern *m_pattern;


        QRect  *m_sampleWindowAttributes;
        QPoint *m_sampleLabelAttributes;
        QRect  *m_patternWidgetAttributes;
        QRect  *m_browserWidgetAttributes;

        /* pots */
        Rotary *m_potiWidget[5];
};

#endif // PAGESUBCHANNEL_H
