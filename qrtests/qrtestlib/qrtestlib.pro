TEMPLATE = lib

QT += widgets testlib
CONFIG += c++11


DESTDIR = ../../bin

HEADERS += \
    qrtestlib.hpp

SOURCES += \
	qrtestlib.cpp

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

include(../../qrgui/qrgui.pri)
