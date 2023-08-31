greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Linpop
TEMPLATE = subdirs
SUBDIRS = components client
CONFIG += ordered

DEFINES += QT_DEPRECATED_WARNINGS
