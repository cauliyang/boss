# this script adds all subprojects to a single build to allow IDEs understand the full project
# structure.
cmake_minimum_required(VERSION 3.14...3.22)

project(boss_all LANGUAGES CXX)
message(STATUS "Building all subprojects for BINARY")
include(cmake/tools.cmake)

option(BUILD_STANDALONE "Build standalone" ON)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_DOCUMENTS "Build DOCUMENTS" OFF)

if(BUILD_STANDALONE)
  add_subdirectory(standalone ${CMAKE_BINARY_DIR}/standalone)
endif()

if(BUILD_TESTS)
  # needed to generate test target
  enable_testing()
  add_subdirectory(test ${CMAKE_BINARY_DIR}/test)
endif()

if(BUILD_DOCUMENTS)
  add_subdirectory(documentation ${CMAKE_BINARY_DIR}/documentation)
endif()
