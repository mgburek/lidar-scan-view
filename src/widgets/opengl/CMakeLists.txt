set(
    HEADERS
    opengl_view.h
    shader_program.h
    opengl_painter.h
    mesh.h
    camera.h
)

set(
    SOURCES
    opengl_view.cpp
    shader_program.cpp
    opengl_painter.cpp
    mesh.cpp
    camera.cpp
)

add_library(opengl STATIC ${SOURCES} ${HEADERS})

target_include_directories(opengl PUBLIC ../../models)

target_link_libraries(opengl PUBLIC Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets)