cmake_minimum_required(VERSION 3.0)
project(picasso)

#####################################################
# SDL2 LOADING
#####################################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

#####################################################
# DIRECTORES
#####################################################

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(BUILD_DIR ${CMAKE_SOURCE_DIR}/bin)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/imgui)

#####################################################
# SOURCE GLOBBING
#####################################################

file(GLOB_RECURSE INTERNAL_SRCS ${SOURCE_DIR}/*.cc
                                ${SOURCE_DIR}/*.h
)

file(GLOB_RECURSE EXTERNAL_SRCS ${EXTERNAL_DIR}/*.cpp
                                ${EXTERNAL_DIR}/*.cc
                                ${EXTERNAL_DIR}/*.h
                                ${EXTERNAL_DIR}/*.c
)

file(GLOB IMGUI_SRCS ${IMGUI_DIR}/*.cpp
                     ${IMGUI_DIR}/*.h
)

set(SOURCES ${INTERNAL_SRCS} ${EXTERNAL_SRCS} ${IMGUI_SRCS})

# Group of files that will send warnings
# TODO(cristiandonosoc): Remove warnings for these files
#                        Instead of just removing -Werror/-WX
file(GLOB_RECURSE NO_WARN_AS_ERROR ${EXTERNAL_DIR}/*
                                   ${INCLUDE_DIR}/*
                                   ${IMGUI_DIR}/*
)

#####################################################
# INCLUDES
#####################################################

include_directories(${INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/imgui)
include_directories(${SDL2_INCLUDE_DIR})

#####################################################
# COMPILER FLAGS
#####################################################

if (UNIX)
  message(FATAL_ERROR "UNIX not supported yet")
else()
  message(STATUS "Setting MSVC flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /EHsc")
endif()

# -Werror Handling
if(CMAKE_CXX_FLAGS MATCHES "-Werror")
  string(REPLACE "-Werror" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  set(ERROR_FLAGS "${ERROR_FLAGS} -Werror")
endif()
if(CMAKE_CXX_FLAGS MATCHES "/WX")
  string(REPLACE "/WX" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  set(ERROR_FLAGS "${ERROR_FLAGS} /WX")
endif()

# We add it warning-as-error only for files that need it
if (ERROR_FLAGS)
  foreach (f ${SOURCES})
    list(FIND NO_WARN_AS_ERROR ${f} _index)
    if (${_index} GREATER -1)
      set_source_files_properties(f COMPILE_FLAGS ${ERROR_FLAGS})
    endif()
  endforeach()
endif()

#####################################################
# TARGET DEFINITIONS
#####################################################

add_executable(picasso ${SOURCES})
target_link_libraries(picasso ${SDL2_LIBRARY})
target_link_libraries(picasso ${OPENGL_LIBRARIES})

#####################################################
# OUTPUT
#####################################################

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BUILD_DIR}")
set(CMAKE_ARCHIVE_LIBRARY_DIRECTORY "${BUILD_DIR}")
set(CMAKE_ARCHIVE_RUNTIME_DIRECTORY "${BUILD_DIR}")
