cmake_minimum_required(VERSION 3.0)
project(picasso)

#####################################################
# FILE GLOBING
#####################################################

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(BUILD_DIR "${CMAKE_SOURCE_DIR}/bin")

file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cc ${SOURCE_DIR}/*.h)

#####################################################
# COMPILER FLAGS
#####################################################

if (UNIX)
  message(FATAL_ERROR "UNIX not supported yet")
else()
  message(STATUS "Setting MSVC flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /EHsc")
endif()

#####################################################
# TARGET DEFINITIONS
#####################################################

add_executable(picasso ${SOURCES})


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BUILD_DIR}")
set(CMAKE_ARCHIVE_LIBRARY_DIRECTORY "${BUILD_DIR}")
set(CMAKE_ARCHIVE_RUNTIME_DIRECTORY "${BUILD_DIR}")
