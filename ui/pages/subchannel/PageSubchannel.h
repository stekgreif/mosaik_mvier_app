#ifndef PAGESUBCHANNEL_H
#define PAGESUBCHANNEL_H

#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>

#include </usr/include/sndfile.h>

#include "ui/pages/subchannel/qcustomplot.h"
#include <ui/pages/subchannel/DrawEnvelope.h>
#include <ui/pages/subchannel/Pattern.h>

#include "Browser.h"


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
        void slot_changePath(int pathId);
        void slot_changePath(QString pathName);
        void slot_moveCursor(int direction);
        void slot_toggleItmeExpansion(void);
        void slot_loadSample(void);
        void slot_loadSampleToPrelisten(void);


    private slots:
        void slot_changePage(void);
        void slot_togglePlayDirection(void);

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


        /** BROWSER **/
        Browser    *m_browser;



        /** sample view **/
        QCustomPlot *m_samplePlot;
        void refreshSamplePlot(void);
        void resetSamplePlot(void);

        QLabel *m_headline;
        QLabel *m_labelVarPathAndName;


        DrawEnvelope *m_envelope;


        QRect  *m_sampleWindowAttributes;
        QPoint *m_sampleLabelAttributes;
        QRect  *m_patternWidgetAttributes;

        /** test **/
        QPushButton *m_btnTestTreeCollapse;
        QPushButton *m_btnTestPlayDirection;

        QImage *m_image;
        QPixmap *m_pixmap;
        QLabel *m_labelImage;
};

#endif // PAGESUBCHANNEL_H
