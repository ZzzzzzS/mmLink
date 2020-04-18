QT       += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 qwt
DEFINES += QWT_DLL

win32:{
    INCLUDEPATH += C:\ProgramFiles\Qt\Qt5.14.0\5.14.0\msvc2017_64\include\Qwt
    INCLUDEPATH += C:\ProgramFiles\opencv\include
    CONFIG(debug, debug|release) LIBS += -L"C:\ProgramFiles\Qt\Qt5.14.0\5.14.0\msvc2017_64\lib" -lqwtd
    CONFIG(release, debug|release) LIBS += -L"C:\ProgramFiles\Qt\Qt5.14.0\5.14.0\msvc2017_64\lib" -lqwt
    CONFIG(debug, debug|release) LIBS += -L"C:\ProgramFiles\opencv\x64\vc16\lib" -lopencv_world410d
    CONFIG(release, debug|release) LIBS += -L"C:\ProgramFiles\opencv\x64\vc16\lib" -lopencv_world410
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
    TCPConfig.cpp \
    filemanagement.cpp \
    main.cpp \
    mainwindow.cpp \
    mmwaveradar.cpp \
    qwtplotshow.cpp \
    uvccamera.cpp

HEADERS += \
    mainwindow.h \
    mmwaveradar.h \
    qwtplotshow.h \
    uvccamera.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    mmlink_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
