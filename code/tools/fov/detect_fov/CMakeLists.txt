cmake_minimum_required(VERSION 2.8)
project( detect_fov )
find_package( OpenCV REQUIRED )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable( detect_fov detect_fov.cpp )
target_link_libraries( detect_fov ${OpenCV_LIBS} )
