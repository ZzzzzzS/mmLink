cmake_minimum_required(VERSION 3.5)

project(mmlink_RadarSimulation)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Gui Network Widgets REQUIRED)

set(SOURCES 
    main.cpp
    mmLink_RadarSimulation.cpp
    mmLink_RadarSimulation.h
    mmLink_RadarSimulation.ui
)

add_executable(mmlink_RadarSimulation ${SOURCES})

target_link_libraries(mmlink_RadarSimulation PRIVATE Qt5::Core Qt5::Gui Qt5::Network Qt5::Widgets)