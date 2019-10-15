cmake_minimum_required(VERSION 3.14)
project(converter)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

#set(TBB_ROOT "C:/Users/User/Desktop/tbb-2019_U9/")
#find_package(TBB REQUIRED COMPONENTS tbb OPTIONAL_COMPONENTS malloc)

set(OpenCV_DIR C:/Users/User/Desktop/opencv/mingw-build/)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

add_executable(converter main.cpp functions/math_functions.cpp functions/math_functions.h functions/util_functions.cpp functions/util_functions.h functions/create_maps.cpp functions/create_maps.h)
add_executable(converter-win main-win.cpp functions/math_functions.cpp functions/math_functions.h functions/util_functions.cpp functions/util_functions.h functions/create_maps.cpp functions/create_maps.h)
add_executable(converter-win-parallel main-win-parallel.cpp functions/math_functions.cpp functions/math_functions.h functions/util_functions.cpp functions/util_functions.h functions/create_maps.cpp functions/create_maps.h)

target_link_libraries(converter ${OpenCV_LIBS})
target_link_libraries(converter-win ${OpenCV_LIBS})
target_link_libraries(converter-win-parallel ${OpenCV_LIBS})