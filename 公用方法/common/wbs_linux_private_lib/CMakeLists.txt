cmake_minimum_required(VERSION 2.8)

project(muduo C CXX)

enable_testing()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

set(CXX_FLAGS
 -g
 -D_FILE_OFFSET_BITS=64
 -DLINUX
 -Wall
 -Wextra
 -Werror
 #-Wconversion
 -Wno-unused-parameter
 #-Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 # -std=c++0x
 -rdynamic
 -fPIC
 )
if(CMAKE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")
#set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(INCLUDE_PATH ${PROJECT_SOURCE_DIR}/src)
 
find_package(Boost REQUIRED)
find_path(ODBC_INCLUDE_DIR sql.h)
find_library(ODBC_LIBRARY NAMES odbc)

if(ODBC_INCLUDE_DIR AND ODBC_LIBRARY)
  message(STATUS "found odbc")
endif()

include_directories(${Boost_INCLUDE_DIRS})

include_directories(${INCLUDE_PATH})

message(STATUS "project_source_dir = " ${PROJECT_SOURCE_DIR})

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

add_subdirectory(odbc)
add_subdirectory(mobiareaqry)
add_subdirectory(paranomgr)


