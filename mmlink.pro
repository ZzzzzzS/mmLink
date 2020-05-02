QT       += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 qwt
DEFINES += QWT_DLL

win32:{
    INCLUDEPATH += C:\ProgramFiles\Qt\Qt5.14.0\5.14.0\msvc2017_64\include\Qwt
    INCLUDEPATH += C:\ProgramFiles\OpenCV\opencv4.3.0\include
    CONFIG(debug, debug|release) LIBS += -L"C:\ProgramFiles\Qt\Qt5.14.0\5.14.0\msvc2017_64\lib" -lqwtd
    CONFIG(release, debug|release) LIBS += -L"C:\ProgramFiles\Qt\Qt5.14.0\5.14.0\msvc2017_64\lib" -lqwt
}

win32:{
    CONFIG(debug, debug|release) LIBS += -L"C:\ProgramFiles\OpenCV\opencv4.3.0\x64\vc16\lib" \
    -lopencv_core430d \
    -lopencv_cudev430d \
    -lopencv_videoio430d

    #CONFIG(release, debug|release) LIBS += -L"C:\ProgramFiles\OpenCV\opencv4.3.0\x64\vc16\lib" -lopencv_world410
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
    aboutwindow.cpp \
    filemanagement.cpp \
    license.cpp \
    main.cpp \
    mainwindow.cpp \
    mmwaveradar.cpp \
    qwtplotshow.cpp \
    uvccamera.cpp

HEADERS += \
    aboutwindow.h \
    license.h \
    mainwindow.h \
    mmwaveradar.h \
    qwtplotshow.h \
    uvccamera.h

FORMS += \
    aboutwindow.ui \
    mainwindow.ui

TRANSLATIONS += \
    mmlink_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
