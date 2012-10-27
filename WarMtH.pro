#-------------------------------------------------
#
# Project created by QtCreator 2012-10-25T00:51:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = warmth
TEMPLATE = app
VERSION = 1.0

DEPENDPATH += . src
INCLUDEPATH += . src

# Input
HEADERS += src/configwindow.h src/mainwindow.h src/authmsgwindow.h
SOURCES += src/configwindow.cpp src/main.cpp src/mainwindow.cpp src/authmsgwindow.cpp

RESOURCES += \
    images/images.qrc


OTHER_FILES += \
    README \
    COPYING \
    AUTHORS \
    TODO

unix {
    target.path = /usr/bin
    INSTALLS += target
}
