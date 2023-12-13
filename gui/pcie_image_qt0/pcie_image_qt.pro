#-------------------------------------------------
#
# Project created by QtCreator 2017-12-25T20:11:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pcie_image_qt
TEMPLATE = app


SOURCES += main.cpp\
    event_thread.cpp \
    pcie_func.c \
    rwif_any.c \
    rwif_bmp.c \
    pcie_image_qt.cpp

HEADERS  += pcie_func.h \
    rwif.h \
    pcie_image_qt.h

FORMS    += mainwindow.ui
