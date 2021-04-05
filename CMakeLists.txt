cmake_minimum_required(VERSION 3.14)

project(graphlayout LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

add_subdirectory(libs)

set(HEADERS_DIR "include")
set(SOURCES_DIR "src")

set(HEADERS
    ${HEADERS_DIR}/igraph.h
    ${HEADERS_DIR}/graphlayout.h
    ${HEADERS_DIR}/graphlineproxymodel.h
    ${HEADERS_DIR}/graphnodesproxymodel.h
    ${HEADERS_DIR}/graphcalculator.h
    ${HEADERS_DIR}/forcedgraphcalculator.h
)

set(SOURCES
    ${SOURCES_DIR}/graphlineproxymodel.cpp
    ${SOURCES_DIR}/graphnodesproxymodel.cpp
    ${SOURCES_DIR}/graphlayout.cpp
    ${SOURCES_DIR}/graphcalculator.cpp
    ${SOURCES_DIR}/forcedgraphcalculator.cpp
)

set(GRAPH_LAYOUT_QML_RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/graph-layout-qml.qrc
)

set(GRAPH_LAYOUT_QML_RESOURCES ${GRAPH_LAYOUT_QML_RESOURCES} PARENT_SCOPE)

add_library(graphlayout ${HEADERS} ${SOURCES} ${GRAPH_LAYOUT_QML_RESOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${HEADERS_DIR})

target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    kmisc
    graphgeometry
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    ENABLE_EXPORTS 1
    WINDOWS_EXPORT_ALL_SYMBOLS 1
)

#initWasmSupport(${PROJECT_NAME} deployWasm.json)
