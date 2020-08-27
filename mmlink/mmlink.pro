#MIT License
#
#Copyright (c) 2020 ZhouZishun
#
#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:
#
#The above copyright notice and this permission notice shall be included in all
#copies or substantial portions of the Software.
#
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#SOFTWARE.


QT       += core gui network sql multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 qwt
DEFINES += QWT_DLL

win32:{

    # disable C4819 warning
    QMAKE_CXXFLAGS_WARN_ON += -wd4819

    INCLUDEPATH += C:\ProgramFiles\Qt\Qt5.14.2\5.14.2\msvc2017_64\include\Qwt
    INCLUDEPATH += C:\ProgramFiles\opencv\opencv4.3.0\include
    CONFIG(debug, debug|release) LIBS += -L"C:\ProgramFiles\Qt\Qt5.14.2\5.14.2\msvc2017_64\lib" -lqwtd
    CONFIG(release, debug|release) LIBS += -L"C:\ProgramFiles\Qt\Qt5.14.2\5.14.2\msvc2017_64\lib" -lqwt

    CONFIG(debug, debug|release) LIBS += -L"C:\ProgramFiles\opencv\opencv4.3.0\lib" \
    -lopencv_core430d \
    -lopencv_videoio430d \

    CONFIG(release, debug|release) LIBS += -L"C:\ProgramFiles\opencv\opencv4.3.0\lib" \
    -lopencv_core430 \
    -lopencv_videoio430 \
}

unix:{
    INCLUDEPATH+= /usr/local/include/opencv4
    INCLUDEPATH+= /usr/local/qwt-6.1.4/include

    LIBS += -L"/usr/local/qwt-6.1.4/lib" -lqwt
    LIBS += -L"/usr/local/lib" \
    -lopencv_core \
    -lopencv_videoio \

}






# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CameraConfig.cpp \
    RadarConfig.cpp \
    aboutwindow.cpp \
    datamanagement.cpp \
    main.cpp \
    mainwindow.cpp \
    mmwaveradar.cpp \
    mmwaveradarfft.cpp \
    qwtplotshow.cpp \
    uvccamera.cpp

HEADERS += \
    aboutwindow.h \
    datamanagement.h \
    mainwindow.h \
    mmwaveradar.h \
    mmwaveradarfft.h \
    qwtplotshow.h \
    uvccamera.h

FORMS += \
    aboutwindow.ui \
    mainwindow.ui

TRANSLATIONS += \
    mmlink_zh_CN.ts\
    mmlink_us_EN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:{
    RESOURCES += \
        windows_resource.qrc
}

unix:{
    RESOURCES += \
        unix_resource.qrc
}


DISTFILES += \
    mmlink_us_EN.ts

RC_ICONS=logo.ico
