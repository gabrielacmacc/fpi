project(FPI_T2)
cmake_minimum_required(VERSION 3.3.2)

SET("OpenCV_DIR" "C:/opt/opencv/build")
set(OpenCV_FOUND 1)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(
    FPI_T2 src/main.cpp 
    src/basic/basic.cpp 
    src/convolution/convolution.cpp 
    src/files/files.cpp 
    src/filters/filters.cpp
)

target_link_libraries(FPI_T2 ${OpenCV_LIBS})