cmake_minimum_required(VERSION 3.14)
project(converter)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(converter main.cpp)
target_link_libraries(converter ${OpenCV_LIBS})