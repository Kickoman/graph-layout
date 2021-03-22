cmake_minimum_required(VERSION 3.5)

project(validation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT KMISC_LIB_TYPE)
    set(KMISC_LIB_TYPE "STATIC")
endif()

file(GLOB_RECURSE PROJECT_SRC ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE PROJECT_HED ${PROJECT_SOURCE_DIR}/*.h)

add_library(kmisc
    ${KMISC_LIB_TYPE}
    ${PROJECT_SRC}
    ${PROJECT_HED}
)


target_include_directories(kmisc PUBLIC
    ktypes/
    common/
    kfunction/)
