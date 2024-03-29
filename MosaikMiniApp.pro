#-------------------------------------------------
#
# Project created by QtCreator 2015-02-21T22:52:27
#
#-------------------------------------------------

QT       += core gui\
            printsupport


#find_package(Qt6 REQUIRED COMPONENTS Core5Compat)
#target_link_libraries(mytarget PRIVATE Qt6::Core5Compat)

greaterThan(QT_MAJOR_VERSION,5): QT += core5compat
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
    ui/pages/subchannel/DrawEnvelope.cpp \
	ui/pages/subchannel/DrawPlayCursor.cpp \
    ui/pages/selection/SelectionPad.cpp \
	ui/pages/subchannel/StepPad.cpp \
    ui/pages/subchannel/Pattern.cpp \
    core/midi/devices/VirtualMidiDevice.cpp \
    core/subchannel/Prelisten.cpp \
    ui/pages/selection/MuteAndSolo.cpp \
    ui/pages/selection/MuteAndSoloPad.cpp \
    ui/pages/subchannel/BrowserTree.cpp \
    ui/pages/subchannel/Browser.cpp \
    ui/pages/selection/VolumeWidget.cpp \
    ui/pages/info/PageInfos.cpp \
    ui/pages/test/PageTest.cpp \
    ui/pages/subchannel/ToolBoxMute.cpp \
    core/midi/devices/MidiDevices.cpp \
    core/midi/mvier/Hwui_01.cpp \
    core/midi/mvier/Hwui_02.cpp \
    core/midi/mvier/RgbwButtons.cpp \
    core/midi/mvier/DeviceIdentifier.cpp \
    core/midi/mvier/Hwui_03.cpp

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
    ui/pages/selection/PageSelection.h \
    ui/pages/subchannel/PageSubchannel.h \
    ui/pages/subchannel/qcustomplot.h \
    ui/UiManager.h \
    ui/pages/subchannel/DrawEnvelope.h \
	ui/pages/subchannel/DrawPlayCursor.h \
    ui/pages/selection/SelectionPad.h \
	ui/pages/subchannel/Pattern.h \
	ui/pages/subchannel/StepPad.h \
    core/midi/devices/VirtualMidiDevice.h \
    core/subchannel/Prelisten.h \
    ui/pages/selection/MuteAndSolo.h \
    ui/pages/selection/MuteAndSoloPad.h \
    ui/pages/subchannel/BrowserTree.h \
    ui/pages/subchannel/Browser.h \
    ui/pages/selection/VolumeWidget.h \
    ui/pages/info/PageInfos.h \
    ui/pages/test/PageTest.h \
    ui/pages/subchannel/ToolBoxMute.h \
    core/midi/devices/MidiDevices.h \
    core/midi/mvier/Hwui_01.h \
    core/midi/mvier/Hwui_02.h \
    core/midi/mvier/RgbwButtons.h \
    core/midi/mvier/DeviceIdentifier.h \
    core/midi/mvier/Hwui_03.h

FORMS    += MosaikMiniApp.ui \
    pagemidi.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    stylesheet.qss \
    txt/_SignalFlow.txt \
    txt/_TodosAndBugs.txt \
    txt/_VersionsAndFeatures.txt \
    txt/_help.txt
