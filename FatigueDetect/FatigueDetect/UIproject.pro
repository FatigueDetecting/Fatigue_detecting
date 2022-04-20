QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += /usr/local/lib/arm-linux-gnueabihf/libopencv_*.so
INCLUDEPATH += \
            ../opencv4

INCLUDEPATH += ../dlibAndModel/dlib
LIBS += -L ../dlibAndModel/dlib

LIBS += -lpthread
LIBS += -lpng -ljpeg
CONFIG += link_pkgconfig
PKGCONFIG += x11
QMAKE_CXXFLAGS += -std=c++11 -DDLIB_PNG_SUPPORT -DDLIB_JPEG_SUPPORT
SOURCES += ../dlibAndModel/dlib/dlib/all/source.cpp
SOURCES += \
    detect.cpp \
    facedetectcnn-data.cpp \
    facedetectcnn-model.cpp \
    facedetectcnn.cpp \
    faceget.cpp \
    main.cpp \
    parallelvideocapture.cpp \
    widget.cpp

HEADERS += \
    CRingBuffer.h \
    detect.h \
    facedetectcnn.h \
    facedetection_export.h \
    faceget.h \
    parallelvideocapture.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
