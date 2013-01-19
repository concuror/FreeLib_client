#-------------------------------------------------
#
# Project created by QtCreator 2012-12-15T20:47:22
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = freeLib
TEMPLATE = app


SOURCES += main.cpp\
        testwindow.cpp \
    libconnector.cpp \
    book.cpp \
    librarymanager.cpp \
    qt-json/json.cpp \
    jsonabstraction.cpp

HEADERS  += testwindow.h \
    libconnector.h \
    book.h \
    librarymanager.h \
    qt-json/json.h \
    jsonabstraction.h

FORMS    +=

OTHER_FILES += \
    README.md \
    LICENSE.txt \
    qt-json/README \
    qt-json/LICENSE \
    qt-json/AUTHORS
