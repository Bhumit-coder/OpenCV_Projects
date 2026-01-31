cmake_minimum_required(VERSION 3.16)

project(Color_Picker
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV REQUIRED)

add_executable(Color_Picker
    src/main.cpp
    src/ColorPicker.cpp
)

target_include_directories(Color_Picker
    PRIVATE
        ${OpenCV_INCLUDE_DIRS}
        src
)

target_link_libraries(Color_Picker
    PRIVATE
        ${OpenCV_LIBS}
)

# Optional: Visual Studio quality-of-life (safe for GSOC)
set_property(TARGET Color_Picker PROPERTY
    VS_DEBUGGER_WORKING_DIRECTORY
    $<TARGET_FILE_DIR:Color_Picker>
)
