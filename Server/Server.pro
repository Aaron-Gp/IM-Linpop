QT       += core gui
QT       += network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client.cpp \
    database.cpp \
    filemanager.cpp \
    main.cpp \
    mainwindow.cpp \
    msg.cpp \
    msganalyzer.cpp \
    sockettask.cpp \
    tcpserver.cpp

HEADERS += \
    Definition.h \
    client.h \
    database.h \
    filemanager.h \
    mainwindow.h \
    msg.h \
    msganalyzer.h \
    sockettask.h \
    tcpserver.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
