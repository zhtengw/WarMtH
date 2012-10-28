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

TRANSLATIONS += \
    l10n/warmth_en.ts  \
    l10n/warmth_zh_CN.ts
OTHER_FILES += \
    README \
    COPYING \
    AUTHORS \
    TODO

icon.files = images/warmth.png
desktop.files = src/warmth.desktop
other.files +=  \
        l10n    \
        README  \
        COPYING \
        AUTHORS \
        warmth.slackbuild
unix {
    icon.path = /usr/share/icons/hicolor/128x128/apps/
    desktop.path = /usr/share/applications/
    other.path = /usr/share/warmth
    target.path = /usr/bin
    INSTALLS += \
        target  \
        icon    \
        desktop \
        other
}
