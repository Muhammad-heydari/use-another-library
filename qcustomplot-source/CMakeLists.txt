cmake_minimum_required(VERSION 3.5)

project(qcustomplot-source LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(qcustomplot-source qcustomplot.cpp)

find_package(Qt5 COMPONENTS Widgets PrintSupport REQUIRED)




target_link_libraries(qcustomplot-source PRIVATE Qt5::Widgets Qt5::PrintSupport)
target_include_directories(qcustomplot-source PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

