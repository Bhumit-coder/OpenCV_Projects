cmake_minimum_required(VERSION 3.10)

project(Documents_Scanner)

find_package(OpenCV REQUIRED)

add_executable(Documents_Scanner
    src/main.cpp
)

target_link_libraries(Documents_Scanner
    ${OpenCV_LIBS}
)
