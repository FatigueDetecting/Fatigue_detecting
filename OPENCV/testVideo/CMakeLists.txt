cmake_minimum_required (VERSION 3.0)
cmake_policy(SET CMP0012 NEW)
PROJECT(demo)

find_package(OpenCV REQUIRED)
message(" opencv version: ${OpenCV_VERSION}")
include_directories(${OpenCV_INCLUDE_DIRS})        # Not needed for CMake >= 2.8.11

ADD_EXECUTABLE(demo main.cpp )
TARGET_LINK_LIBRARIES(demo ${OpenCV_LIBS})