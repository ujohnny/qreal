#-------------------------------------------------
#
# Project created by QtCreator 2014-03-28T18:13:21
#
#-------------------------------------------------

QT       += testlib

TARGET = tst_demoguitesttest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11
TEMPLATE = app

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

!macx {
        QMAKE_LFLAGS += -Wl,-O1,-rpath,$$PWD/../bin/
        QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../bin/thirdparty/
}

if (equals(QMAKE_CXX, "g++") : !macx) {
        QMAKE_LFLAGS += -Wl,-E
}

CONFIG += rpath_libdirs

DESTDIR = $$PWD/../../bin

SOURCES += tst_demoguitesttest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

LIBS += -lpthread

include(../../qrgui/qrgui.pri)
