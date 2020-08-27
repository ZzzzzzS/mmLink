cmake_minimum_required(VERSION 3.5)

project(mmlink)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# find Qt5 package
find_package(Qt5 COMPONENTS Core Gui Network Sql Multimedia Widgets  LinguistTools REQUIRED)

if(Qt5_FOUND)
    message("Qt5 FOUND")
else()
    message(FATAL_ERROR "Missing Qt5 components!")
endif()

# find OpenCV package
find_package(OpenCV REQUIRED)

if(OpenCV_FOUND)
    message("OpenCV FOUND")
else()
    message(FATAL_ERROR "Missing OpenCV components!")
endif()

# add Qwt library
include_directories(${PROJECT_SOURCE_DIR} /usr/local/qwt-6.1.5/include)

# add source code and header
set(SOURCES
    CameraConfig.cpp 
    RadarConfig.cpp 
    aboutwindow.cpp 
    datamanagement.cpp 
    main.cpp 
    mainwindow.cpp 
    mmwaveradar.cpp 
    mmwaveradarfft.cpp 
    qwtplotshow.cpp 
    uvccamera.cpp
    aboutwindow.h 
    datamanagement.h 
    mainwindow.h 
    mmwaveradar.h 
    mmwaveradarfft.h 
    qwtplotshow.h 
    uvccamera.h
    )


# add resouces
set(UI 
    aboutwindow.ui
    mainwindow.ui)

if(WIN32)
    set(RESOURCE  windows_resource.qrc)
elseif(UNIX)
    set(RESOURCE unix_resource.qrc)
endif()

# add translation files
set(TS_FILES mmlink_zh_CN.ts mmlink_us_EN.ts)
qt5_add_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
add_custom_target(cleants COMMAND rm *.ts)

# add exec and lib

add_executable(mmlink ${SOURCES} ${UI} ${RESOURCE} ${TS_FILES})

target_link_libraries(mmlink PRIVATE
        Qt5::Core 
        Qt5::Gui 
        Qt5::Network
        Qt5::Sql
        Qt5::Multimedia 
        Qt5::Widgets
        ${OpenCV_LIBS}
        libqwt.so
)
