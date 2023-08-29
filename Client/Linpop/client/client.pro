QT += core gui widgets network sql
TEMPLATE = app
CONFIG += c++11
SOURCES = main.cpp \
    cabstractemojitable.cpp \
    listbar.cpp \
    mainbar.cpp \
    mainwindow.cpp \
    profilemanager.cpp \
    qnchatmessage.cpp \
    sidebar.cpp \
    tcpclient.cpp \
    tcpserver.cpp
HEADERS = mainwindow.h \
    cabstractemojitable.h \
    listbar.h \
    mainbar.h \
    profilemanager.h \
    qnchatmessage.h \
    sidebar.h \
    tcpclient.h \
    tcpserver.h

LIBS += $$top_builddir/components/$(OBJECTS_DIR)/libcomponents.a
INCLUDEPATH += $$top_srcdir/components/
TARGET = client-exe
PRE_TARGETDEPS += $$top_builddir/components/$(OBJECTS_DIR)/libcomponents.a

FORMS += \
    mainwindow.ui

RESOURCES += \
    assets.qrc
