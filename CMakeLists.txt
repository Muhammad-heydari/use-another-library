cmake_minimum_required(VERSION 3.5)

project(untitled115 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

add_subdirectory(qcustomplot-source)

add_executable(untitled115
    main.cpp
    Dialog.cpp
    Dialog.h
    Dialog.ui
 )


target_link_libraries(untitled115 PRIVATE Qt5::Widgets Qt5::PrintSupport qcustomplot-source)

