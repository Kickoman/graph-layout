cmake_minimum_required(VERSION 3.14)

project(graphlayout LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs/kmisc)
add_subdirectory(libs/GraphGeometry)

set(HEADERS_DIR "include")
set(SOURCES_DIR "src")

set(HEADERS
    ${HEADERS_DIR}/igraph.h
    ${HEADERS_DIR}/graphcalculator.h
)

set(SOURCES
    ${SOURCES_DIR}/graphcalculator.cpp
)


add_library(graphlayout ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${HEADERS_DIR} ${KMISC_INCLUDES})

target_link_libraries(${PROJECT_NAME}
    graphgeometry
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    ENABLE_EXPORTS 1
    WINDOWS_EXPORT_ALL_SYMBOLS 1
)

#initWasmSupport(${PROJECT_NAME} deployWasm.json)
