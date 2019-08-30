#-------------------------------------------------
#
# Project created by QtCreator 2019-03-26T11:27:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = face_ui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
INCLUDEPATH += E:\opencv\install\install\include
INCLUDEPATH += E:\opencv\install\install\include\opencv2
#INCLUDEPATH += E:\opencv\install\opencv2

#LIBS += -L E:\opencv\install\lib\libopencv_*.a
LIBS += -L E:/opencv/install/install/x86/mingw/lib/lib*
