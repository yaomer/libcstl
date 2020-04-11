cmake_minimum_required (VERSION 2.8)

project (libcstl)

set (CMAKE_C_FLAGS "-std=c99 -Wall -O2")

set (PROJECT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set (PROJECT_SRC_DIR "${PROJECT_SOURCE_DIR}/src")

file (GLOB SRC_HEADERS ${PROJECT_INCLUDE_DIR}/*.h)
file (GLOB SRC_FILES ${PROJECT_SRC_DIR}/*.c)

add_library (cstl STATIC ${SRC_FILES})

install(TARGETS cstl
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

install(FILES ${SRC_HEADERS} DESTINATION include/libcstl)
