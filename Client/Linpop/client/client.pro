QT += core gui widgets network sql
TEMPLATE = app
CONFIG += c++11
SOURCES = main.cpp \
    clientdatabase.cpp \
    listbar.cpp \
    login.cpp \
    mainbar.cpp \
    mainwindow.cpp \
    msganalyzer.cpp \
    profilemanager.cpp \
    qnchatmessage.cpp \
    sidebar.cpp \
    tcpclient.cpp \
    tcpserver.cpp
HEADERS = mainwindow.h \
    Definition.h \
    clientdatabase.h \
    listbar.h \
    login.h \
    mainbar.h \
    msganalyzer.h \
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
    login.ui \
    mainwindow.ui

RESOURCES += \
    assets.qrc
