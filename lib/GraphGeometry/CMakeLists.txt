cmake_minimum_required(VERSION 3.5)

project(GraphGeometry LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT GRAPH_GEOMETRY_LIB_TYPE)
    set(GRAPH_GEOMETRY_LIB_TYPE "STATIC")
endif()

set(HEADERS_DIR include/)
set(SOURCES_DIR src/)

set(HEADERS
    ${HEADERS_DIR}/line.h
    ${HEADERS_DIR}/section.h
    ${HEADERS_DIR}/twodvector.h
    ${HEADERS_DIR}/point.h
    ${HEADERS_DIR}/mathematics.h
)

set(SOURCES
    ${SOURCES_DIR}/line.cpp
    ${SOURCES_DIR}/section.cpp
    ${SOURCES_DIR}/twodvector.cpp
    ${SOURCES_DIR}/point.cpp
)

add_library(graphgeometry ${GRAPH_GEOMETRY_LIB_TYPE} ${HEADERS} ${SOURCES})
target_include_directories(graphgeometry PUBLIC ${HEADERS_DIR})
