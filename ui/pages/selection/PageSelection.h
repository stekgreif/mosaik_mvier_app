#ifndef PAGESELECTION_H
#define PAGESELECTION_H

#include <QMap>
#include <QWidget>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>

#include "ui/pages/selection/SelectionPad.h"
#include "ui/pages/selection/MuteAndSolo.h"


class UiManager;
class QLabel;

class PageSelection : public QWidget
{
    Q_OBJECT

    public:
        explicit PageSelection(UiManager *parent);
        ~PageSelection();
        void refresh(void);
        void refreshTiles(void);
        void refreshMutePads(void);
        void refreshSampleNames(void);
        void refreshSampleVolume(void);

    signals:
        void signal_subchannelSelectionPadPressed(int id);


    private slots:
        void m_slot_selectionChanged(int id);


    public slots:
        void slot_setExclusiveSubchannelSelection(int id);
        void toggleMuteAndSolo();

    private:
        SelectionPad *m_pad[64];
        int m_lastSelectedSubchannel;
        QLabel *m_channelSelectionFrame;

        MuteAndSolo *m_muteAndSolo;
        bool m_muteAndSoloVisible;

    protected:
        void paintEvent(QPaintEvent *event);

};

#endif // PAGESELECTION_H
