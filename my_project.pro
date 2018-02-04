#-------------------------------------------------
#
# Project created by QtCreator 2017-07-14T11:20:20
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = my_project
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    mouse_position.cpp \
    featuredata.cpp \
    display_image.cpp \
    myqlabel.cpp \
    DetectorDatabase.cpp \
    About.cpp \
    Contents.cpp \
    ObjectSize.cpp

HEADERS  += mainwindow.h \
    mouse_position.h \
    featuredata.h \
    display_image.h \
    myqlabel.h \
    DetectorDatabase.h \
    About.h \
    Contents.h \
    ObjectSize.h

FORMS    += mainwindow.ui \
    objectdialog.ui \
    featuredata.ui \
    display_image.ui \
    DetectorDatabase.ui \
    About.ui \
    Contents.ui \
    ObjectSize.ui

RESOURCES +=

INCLUDEPATH += C://opencv//sources//release//install//include
INCLUDEPATH += C://opencv//sources//include//opencv2
#LIBS += -LC://opencv//sources//release//bin//*.dll

LIBS += C:\opencv\sources\release\bin\libopencv_calib3d249.dll
LIBS += C:\opencv\sources\release\bin\libopencv_contrib249.dll
LIBS += C:\opencv\sources\release\bin\libopencv_core249.dll
LIBS += C:\opencv\sources\release\bin\libopencv_features2d249.dll
LIBS += C:\opencv\sources\release\bin\libopencv_flann249.dll
LIBS += C:\opencv\sources\release\bin\libopencv_gpu249.dll
LIBS += C:\opencv\sources\release\bin\libopencv_highgui249.dll
LIBS += C:\opencv\sources\release\bin\libopencv_imgproc249.dll
LIBS += C:\opencv\sources\release\bin\libopencv_legacy249.dll
LIBS += C:\opencv\sources\release\bin\libopencv_ml249.dll
LIBS += C:\opencv\sources\release\bin\libopencv_nonfree249.dll
LIBS += C:\opencv\sources\release\bin\libopencv_objdetect249.dll
LIBS += C:\opencv\sources\release\bin\libopencv_ocl249.dll
LIBS += C:\opencv\sources\release\bin\libopencv_photo249.dll
LIBS += C:\opencv\sources\release\bin\libopencv_stitching249.dll
LIBS += C:\opencv\sources\release\bin\libopencv_superres249.dll
LIBS += C:\opencv\sources\release\bin\libopencv_video249.dll
LIBS += C:\opencv\sources\release\bin\libopencv_videostab249.dll
LIBS += C:\opencv\sources\release\bin\opencv_ffmpeg249.dll


win32: LIBS += -L$$PWD/../../Desktop/lib/ -llibmysql

INCLUDEPATH += $$PWD/../../Desktop
DEPENDPATH += $$PWD/../../Desktop
