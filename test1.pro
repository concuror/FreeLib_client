#-------------------------------------------------
#
# Project created by QtCreator 2012-12-15T20:47:22
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test1
TEMPLATE = app


SOURCES += main.cpp\
        testwindow.cpp \
    libconnector.cpp \
    book.cpp \
    librarymanager.cpp

HEADERS  += testwindow.h \
    libconnector.h \
    book.h \
    librarymanager.h

FORMS    += testwindow.ui

LIBS += -ljson

OTHER_FILES += \
    README.md \
    LICENSE.txt
