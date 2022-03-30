QT       += core gui



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += /usr/local/lib/libopencv_*.so

INCLUDEPATH += \
            /usr/local/include/opencv4


SOURCES += \
    detection.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    detection.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# dlib库的配置
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../dlib/dlib-19.23/examples/build/dlib_build/release/ -ldlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../dlib/dlib-19.23/examples/build/dlib_build/debug/ -ldlib
else:unix: LIBS += -L$$PWD/../../dlib/dlib-19.23/examples/build/dlib_build/ -ldlib

INCLUDEPATH += $$PWD/../../dlib/dlib-19.23
DEPENDPATH += $$PWD/../../dlib/dlib-19.23

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../dlib/dlib-19.23/examples/build/dlib_build/release/libdlib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../dlib/dlib-19.23/examples/build/dlib_build/debug/libdlib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../dlib/dlib-19.23/examples/build/dlib_build/release/dlib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../dlib/dlib-19.23/examples/build/dlib_build/debug/dlib.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../dlib/dlib-19.23/examples/build/dlib_build/libdlib.a
