cmake_minimum_required(VERSION 3.14)
project(Graph_DataStructureCourseDeign)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_PREFIX_PATH /Users/nole/SoftwaresFromInternet/5.13.0/clang_64/lib/cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(SOURCES main.cpp VertexItem.cpp ArcItem.cpp Resources.qrc)
set(HEADERS GraphModel.h VertexItem.h ArcItem.h VertexListItem.h GraphItem.h ApplicationWindow.h
        AdjointSingleListItem.h AdjointNodeItem.h AdjointListItem.h)

add_executable(FreeGraph ${SOURCES} ${HEADERS})

target_link_libraries(FreeGraph Qt5::Widgets)
target_link_libraries(FreeGraph Qt5::Core)
