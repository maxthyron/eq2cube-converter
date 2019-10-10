cmake_minimum_required(VERSION 3.14)
project(converter)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(OpenCV_DIR C:/Users/User/Desktop/opencv/mingw-build/)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(converter main.cpp functions/math_functions.cpp functions/math_functions.h functions/util_functions.cpp functions/util_functions.h)
add_executable(converter-win main-win.cpp functions/math_functions.cpp functions/math_functions.h functions/util_functions.cpp functions/util_functions.h)
add_executable(converter-win-parallel main-win-parallel.cpp functions/math_functions.cpp functions/math_functions.h functions/util_functions.cpp functions/util_functions.h)

set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
target_link_libraries(converter ${OpenCV_LIBS})
target_link_libraries(converter-win ${OpenCV_LIBS})
target_link_libraries(converter-win-parallel ${OpenCV_LIBS})