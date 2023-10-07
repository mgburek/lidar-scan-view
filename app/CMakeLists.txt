add_executable(${MAIN_EXECUTABLE_NAME} main.cpp)

# Qt related libraries
target_link_libraries(
    ${MAIN_EXECUTABLE_NAME} 
    PRIVATE 
    Qt6::Widgets 
    Qt6::OpenGL 
    Qt6::OpenGLWidgets 
    Qt6::SerialPort
)

# my own libraries
target_link_libraries(
    ${MAIN_EXECUTABLE_NAME} 
    PRIVATE 
    models 
    widgets 
    controllers
)