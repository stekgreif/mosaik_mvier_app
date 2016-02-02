#-------------------------------------------------
#
# Project created by QtCreator 2015-02-21T22:52:27
#
#-------------------------------------------------

QT       += core gui\
			printsupport\
			webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MosaikMiniApp
TEMPLATE = app

LIBS    +=  -lasound\
            -lpthread\
            -lsndfile


QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp\
	MosaikMiniApp.cpp\
	Settings.cpp \
	core/audio/AlsaPcm.cpp \
    core/ppthread.cpp \
	core/subchannel/SubchannelManager.cpp \
	core/subchannel/Subchannel.cpp \
	core/subchannel/AudioParameter.cpp \
	core/midi/MidiIn.cpp \
	core/midi/MidiOut.cpp \
	core/midi/MidiInfo.cpp\
	core/midi/MidiManager.cpp \
	core/midi/devices/MidiDevice.cpp \
	core/midi/devices/MosaikMini.cpp \
	core/midi/devices/SparkLe.cpp \
	core/subchannel/Sequencer.cpp \
	core/subchannel/Envelope.cpp \
	core/subchannel/Sample.cpp \
	ui/UiManager.cpp \
    ui/pages/selection/PageSelection.cpp \
    ui/pages/subchannel/PageSubchannel.cpp \
    ui/pages/subchannel/qcustomplot.cpp \
    ui/pages/subchannel/mypot.cpp \
    ui/pages/subchannel/rotary.cpp \
    ui/pages/subchannel/DrawEnvelope.cpp \
	ui/pages/subchannel/DrawPlayCursor.cpp \
    ui/pages/selection/SelectionPad.cpp \
	ui/pages/subchannel/StepPad.cpp \
    ui/pages/browser/PageBrowser.cpp \
    ui/pages/subchannel/Pattern.cpp \
	ui/sidepanels/SubchannelPreview.cpp \
    core/midi/devices/VirtualMidiDevice.cpp \
    ui/pages/browser/SampleBrowser.cpp \
    core/subchannel/Prelisten.cpp \
    ui/pages/selection/MuteAndSolo.cpp \
    ui/pages/selection/MuteAndSoloPad.cpp \
    ui/pages/subchannel/BrowserTree.cpp \
    ui/pages/subchannel/Browser.cpp \
    ui/pages/selection/VolumeWidget.cpp

HEADERS  += MosaikMiniApp.h \
	Settings.h \
	MosaikTypes.h \
    audio/AlsaPcm.h \
    core/audio/AlsaPcm.h \
    core/ppthread.h \
	core/midi/MidiOut.h \
	core/midi/MidiInfo.h \
	core/midi/MidiIn.h \
	core/midi/MidiNames.h \
	core/midi/MidiManager.h \
	core/midi/devices/MosaikMini.h \
	core/midi/devices/MidiDevice.h \
	core/midi/devices/SparkLe.h \
	core/subchannel/SubchannelManager.h \
	core/subchannel/Subchannel.h \
	core/subchannel/AudioParameter.h \
	core/subchannel/Sequencer.h \
	core/subchannel/Envelope.h \
	core/subchannel/Sample.h \
    ui/pages/youtube/Youtube.h \
    ui/pages/selection/PageSelection.h \
    ui/pages/subchannel/PageSubchannel.h \
    ui/pages/subchannel/qcustomplot.h \
    ui/sidepanels/SubchannelPreview.h \
    ui/UiManager.h \
    ui/pages/subchannel/mypot.h \
    ui/pages/subchannel/rotary.h \
    ui/pages/subchannel/DrawEnvelope.h \
	ui/pages/subchannel/DrawPlayCursor.h \
    ui/pages/selection/SelectionPad.h \
    ui/pages/browser/PageBrowser.h \
	ui/pages/subchannel/Pattern.h \
	ui/pages/subchannel/StepPad.h \
    core/midi/devices/VirtualMidiDevice.h \
    ui/widgets/test.h \
    ui/pages/browser/SampleBrowser.h \
    core/subchannel/Prelisten.h \
    ui/pages/selection/MuteAndSolo.h \
    ui/pages/selection/MuteAndSoloPad.h \
    ui/pages/subchannel/BrowserTree.h \
    ui/pages/subchannel/Browser.h \
    ui/pages/selection/VolumeWidget.h

FORMS    += MosaikMiniApp.ui \
    pagemidi.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    stylesheet.qss \
    txt/SignalFlow.txt \
    txt/VersionsAndFeatures.txt \
    txt/HowTos.txt \
    txt/cppFragen
