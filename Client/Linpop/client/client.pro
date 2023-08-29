QT += core gui widgets network sql
TEMPLATE = app
CONFIG += c++14
SOURCES = main.cpp \
    cabstractemojitable.cpp \
    clientdatabase.cpp \
    clienttoserver.cpp \
    emojitab.cpp \
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
    cabstractemojitable.h \
    clientdatabase.h \
    clienttoserver.h \
    emojitab.h \
    global.h \
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
