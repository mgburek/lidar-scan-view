cmake_minimum_required(VERSION 3.26.4)

project(
    Lidar3DScanner
    LANGUAGES CXX 
    VERSION 1.0.0
    )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_DIR "C:\\Qt\\6.4.3\\msvc2019_64\\lib\\cmake\\Qt6")
set(Qt6_DIR "C:\\Qt\\6.4.3\\msvc2019_64\\lib\\cmake\\Qt6")

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS OpenGL)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)
find_package(Qt6 REQUIRED COMPONENTS SerialPort)

set(MAIN_EXECUTABLE_NAME lidarscanview)

add_subdirectory(./app)
add_subdirectory(./src)