TEMPLATE = subdirs

CONFIG += c++11

SUBDIRS += \
	demoGuiTest \
	qrtestlib

demoGuiTest.depends = qrtestlib


