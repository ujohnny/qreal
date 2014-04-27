#-------------------------------------------------
#
# Project created by QtCreator 2014-03-28T18:13:21
#
#-------------------------------------------------

QT       += testlib

TARGET = tst_demoguitestset
CONFIG   += console c++11 rpath_libdirs

CONFIG   -= app_bundle

LIBS += -L$$PWD/../../bin -lqrtestlib

TEMPLATE = app

!macx {
	QMAKE_LFLAGS += -Wl,-O1,-rpath,$$PWD/../../bin/
	QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../../bin/thirdparty/
}

if (equals(QMAKE_CXX, "g++") : !macx) {
        QMAKE_LFLAGS += -Wl,-E
}

DESTDIR = $$PWD/../../bin

SOURCES += \
    tst_demoguitestset.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

include(../../qrgui/qrgui.pri)

HEADERS +=
