cmake_minimum_required(VERSION 3.16)

project(Russian_Number_Plate_Detector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(Russian_Number_Plate_Detector
    src/main.cpp
)

target_link_libraries(Russian_Number_Plate_Detector
    ${OpenCV_LIBS}
)
