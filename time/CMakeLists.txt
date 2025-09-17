cmake_minimum_required(VERSION 3.16)
project(ClockApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2)

qt6_add_executable(ClockApp
    main.cpp
    process.cpp

    process.h
)

target_link_libraries(ClockApp PRIVATE Qt6::Quick Qt6::QuickControls2)

file(COPY ui.qml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})