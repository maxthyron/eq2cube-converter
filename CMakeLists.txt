cmake_minimum_required(VERSION 3.10)
project(converter)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUI ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
# set(CMAKE_PREFIX_PATH "/Users/thyron/Qt/5.11.2/clang_64/lib/cmake")
set(Qt5_DIR "D:/Qt/5.13.1/mingw73_32/lib/cmake/Qt5")

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

# set(TBB_ROOT "/usr/local/Cellar/tbb/2019_U8/")
# find_package(TBB REQUIRED COMPONENTS tbb OPTIONAL_COMPONENTS malloc)


set(OpenCV_DIR D:/builds/opencv-build/)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

set(SOURCE_FUNCTIONS functions/math_functions.cpp functions/math_functions.h functions/util_functions.cpp functions/util_functions.h functions/create_maps.cpp functions/create_maps.h)
set(SOURCE_APP app/window.cpp app/window.h)

add_executable(converter main.cpp ${SOURCE_FUNCTIONS})
add_executable(converter-win main-win.cpp ${SOURCE_FUNCTIONS})
add_executable(converter-win-parallel main-win-parallel.cpp ${SOURCE_FUNCTIONS} functions/thread_functions.cpp functions/thread_functions.h)
add_executable(converter-app app.cpp ${SOURCE_APP} ${SOURCE_FUNCTIONS} ${UIS} functions/thread_functions.cpp functions/thread_functions.h)


target_link_libraries(converter ${OpenCV_LIBS})
target_link_libraries(converter-win ${OpenCV_LIBS})
target_link_libraries(converter-win-parallel ${OpenCV_LIBS}])
target_link_libraries(converter-app ${OpenCV_LIBS} ${Qt5Widgets_LIBRARIES})
