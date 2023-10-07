set(
    HEADERS
    controller.h
)

set(
    SOURCES
    controller.cpp
)

qt6_wrap_cpp(SOURCES controller.h) # moc generation

add_library(controllers STATIC ${SOURCES} ${HEADERS})

target_include_directories(controllers PUBLIC ../widgets/)

target_link_libraries(
    controllers 
    PRIVATE 
    Qt6::SerialPort 
    Qt6::Widgets 
    models 
    widgets
)
