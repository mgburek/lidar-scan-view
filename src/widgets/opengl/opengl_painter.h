#ifndef OPENGL_PAINTER_H
#define OPENGL_PAINTER_H

#include <QOpenGLExtraFunctions>

#include "shader_program.h"
#include "mesh.h"

// #####  ####   #####  #   #  #####  #         ####    ###   #  #   #  #####  #####  ####
// #   #  #   #  #      ##  #  #      #         #   #  #   #  #  ##  #    #    #      #   #
// #   #  ####   #####  # # #  #  ##  #         ####   #####  #  # # #    #    #####  ####
// #   #  #      #      #  ##  #   #  #         #      #   #  #  #  ##    #    #      #  #
// #####  #      #####  #   #  #####  #####     #      #   #  #  #   #    #    #####  #   #

class OpenGLPainter : QOpenGLExtraFunctions
{
public:
    OpenGLPainter(QOpenGLContext *context);

    const GLuint getProgram() const { _program.get(); }
    void paintMesh(float x, float y, float z, double scale = 1.0);
    void prepareForPainting();
    void resetAfterPainting();

private:
    QOpenGLContext *_context;
    ShaderProgram _program;
    Mesh _mesh;
};

#endif // OPENGL_PAINTER_H