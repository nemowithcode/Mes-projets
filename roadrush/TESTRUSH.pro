QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# CONFIG += c++17
CONFIG += relocatable

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += src/car.cpp
SOURCES += src/events.cpp
SOURCES += src/game.cpp
SOURCES += src/hitbox.cpp
SOURCES += src/interface.cpp
SOURCES += src/maintextuel.cpp
SOURCES += src/mainwindow.cpp
SOURCES += src/obstacle.cpp
SOURCES += src/perks.cpp
SOURCES += src/position.cpp
SOURCES += src/savemanager.cpp
SOURCES += src/main.cpp

HEADERS += head/car.h
HEADERS += head/events.h
HEADERS += head/game.h
HEADERS += head/hitbox.h
HEADERS += head/interface.h
HEADERS += head/maintextuel.h
HEADERS += head/mainwindow.h
HEADERS += head/obstacle.h
HEADERS += head/perks.h
HEADERS += head/position.h
HEADERS += head/savemanager.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    TESTRUSH_fr_FR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
