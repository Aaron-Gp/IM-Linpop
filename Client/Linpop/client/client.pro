QT += core gui widgets
TEMPLATE = app
CONFIG += c++11
SOURCES = main.cpp \
    mainwindow.cpp
HEADERS = mainwindow.h

LIBS += $$top_builddir/components/$(OBJECTS_DIR)/libcomponents.a
INCLUDEPATH += $$top_srcdir/components/
TARGET = client-exe
PRE_TARGETDEPS += $$top_builddir/components/$(OBJECTS_DIR)/libcomponents.a

FORMS += \
    mainwindow.ui

RESOURCES += \
    assets.qrc
