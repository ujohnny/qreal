include (qreal.pri)

CONFIG += c++11
TEMPLATE = subdirs
SUBDIRS += \
	qrtests

qrtests.depends = \
	qrgui \
	plugins
