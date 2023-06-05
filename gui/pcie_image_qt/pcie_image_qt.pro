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

LIBS +=-L/usr/lib/x86_64-linux-gnu/ \
-lopencv_calib3d \
-lopencv_core \
-lopencv_features2d \
-lopencv_flann \
-lopencv_highgui \
-lopencv_imgproc \
-lopencv_ml \
-lopencv_objdetect \
-lopencv_photo \
-lopencv_stitching \
-lopencv_superres \
-lopencv_ts \
-lopencv_video \
-lopencv_videostab
