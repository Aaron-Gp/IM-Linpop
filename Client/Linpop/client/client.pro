QT += core gui widgets
TEMPLATE = app
CONFIG += c++11
SOURCES = main.cpp \
    listbar.cpp \
    mainbar.cpp \
    mainwindow.cpp \
    profilemanager.cpp \
    qnchatmessage.cpp \
    sidebar.cpp
HEADERS = mainwindow.h \
    listbar.h \
    mainbar.h \
    profilemanager.h \
    qnchatmessage.h \
    sidebar.h

LIBS += $$top_builddir/components/$(OBJECTS_DIR)/libcomponents.a
INCLUDEPATH += $$top_srcdir/components/
TARGET = client-exe
PRE_TARGETDEPS += $$top_builddir/components/$(OBJECTS_DIR)/libcomponents.a

FORMS += \
    mainwindow.ui

RESOURCES += \
    assets.qrc
