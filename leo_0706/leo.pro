#-------------------------------------------------
#
# Project created by QtCreator 2020-06-04T10:19:41
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = leo
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    muscle_setting.cpp \
    emg_data.cpp

HEADERS  += widget.h \
    muscle_setting.h \
    emg_data.h

FORMS    += widget.ui \
    muscle_setting.ui \
    emg_data.ui

DISTFILES +=
