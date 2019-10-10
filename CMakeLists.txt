cmake_minimum_required(VERSION 3.14)
project(converter)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(OpenCV_DIR C:/Users/User/Desktop/opencv/mingw-build/)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(converter main.cpp)
add_executable(converter-win main-win.cpp)

set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs cvui)
target_link_libraries(converter ${OpenCV_LIBS})
target_link_libraries(converter-win ${OpenCV_LIBS})