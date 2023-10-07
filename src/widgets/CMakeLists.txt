add_subdirectory(opengl)

set(
    HEADERS
    main_window.h
    menu_bar.h
    logs_view.h
    status_bar.h
    actions.h
)

set(
    SOURCES
    main_window.cpp
)

add_library(widgets STATIC ${SOURCES} ${HEADERS})

target_include_directories(widgets PUBLIC ./opengl/)

target_link_libraries(widgets PUBLIC Qt6::Widgets)
target_link_libraries(widgets PUBLIC opengl)
