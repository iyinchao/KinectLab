#-------------------------------------------------
#
# Project created by QtCreator 2015-07-11T10:27:51
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KinectLab
TEMPLATE = app


SOURCES += main.cpp \
    kinectlab.cpp \
    kl2dview.cpp \
    klcontroller.cpp \
    klcheckthread.cpp \
    klctrlpanel.cpp \
    module/klmbase.cpp \
    module/klmknft.cpp \
    klglview.cpp \
    klglutil.cpp

HEADERS  += \
    kinectlab.h \
    kl2dview.h \
    klcontroller.h \
    klcheckthread.h \
    klctrlpanel.h \
    klcommon.h \
    module/klmbase.h \
    module/klmknft.h \
    klglview.h \
    klglutil.h

FORMS    +=

INCLUDEPATH += "C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/inc"

DEPENDPATH += "C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/inc"

LIBS += "C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/Lib/x64/Kinect20.lib"
LIBS += "C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/Lib/x64/Kinect20.Face.lib"

RC_ICONS = res/main.ico
#LIBS += -l"C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/Lib/x86/Kinect20.Face.lib"

#win32:CONFIG(release, debug|release): LIBS += "C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/Lib/x86/Kinect20.lib"

#else:win32:CONFIG(debug, debug|release): LIBS += "C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/Lib/x86/Kinect20d.lib"

#else:unix: LIBS += "C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/Lib/x86/lKinect20.lib"

#win32: LIBS += -lKinect20

RESOURCES += \
    kinectlab.qrc

OTHER_FILES += \
    README.md \
    TODO.md
