project(FPI_T3)
cmake_minimum_required(VERSION 3.3.2)

SET("OpenCV_DIR" "C:/opt/opencv/build")
SET(OpenCV_FOUND 1)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(
    FPI_T3 src/main.cpp 
    src/base/base.cpp 
    src/convolution/convolution.cpp 
    src/files/files.cpp 
    src/filters/filters.cpp
    src/histogram/histogram.cpp
    src/transform/transform.cpp
    src/video/video.cpp
)

target_link_libraries(FPI_T3 ${OpenCV_LIBS})