cmake_minimum_required(VERSION 3.14)
project(Graph_DataStructureCourseDeign)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_PREFIX_PATH /Users/nole/SoftwaresFromInternet/5.13.0/clang_64/lib/cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

add_executable(Graph_DataStructureCourseDeign main.cpp GraphModel.h GraphView.h NodeItem.h ArcItem.h AbstractContainerView.h)

target_link_libraries(Graph_DataStructureCourseDeign Qt5::Widgets)
target_link_libraries(Graph_DataStructureCourseDeign Qt5::Core)
