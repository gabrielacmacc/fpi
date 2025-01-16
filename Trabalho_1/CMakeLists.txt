cmake_minimum_required(VERSION 3.12.0)
project(FPI_T1)

SET("OpenCV_DIR" "C:/opt/opencv/build")
set(OpenCV_FOUND 1)
find_package(OpenCV REQUIRED)
#find_package(OpenCV REQUIRED PATHS "C:/opt/opencv/build" NO_DEFAULT_PATH)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(FPI_T1 src/main.cpp src/imageProcessing/imageProcessing.cpp src/filesOperations/filesOperations.cpp)

target_link_libraries(FPI_T1 ${OpenCV_LIBS})