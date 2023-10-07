#include "opengl_painter.h"

#include <QMatrix4x4>

// #####  ####   #####  #   #  #####  #         ####    ###   #  #   #  #####  #####  ####
// #   #  #   #  #      ##  #  #      #         #   #  #   #  #  ##  #    #    #      #   #
// #   #  ####   #####  # # #  #  ##  #         ####   #####  #  # # #    #    #####  ####
// #   #  #      #      #  ##  #   #  #         #      #   #  #  #  ##    #    #      #  #
// #####  #      #####  #   #  #####  #####     #      #   #  #  #   #    #    #####  #   #

OpenGLPainter::OpenGLPainter(QOpenGLContext *context)
    : QOpenGLExtraFunctions(context),
      _context{context},
      _program(context),
      _mesh(context)
{
    if (_context)
    {
        initializeOpenGLFunctions();

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1, 0.0, 0.1, 1.0);
    }
}

void OpenGLPainter::paintMesh(float x, float y, float z, double scale)
{
    QMatrix4x4 model{};
    
    model.translate(x, y, z);
    model.scale(scale);

    GLuint attr_id = glGetUniformLocation(_program.get(), "model");
    glUniformMatrix4fv(attr_id, 1, GL_FALSE, model.constData());

    _mesh.drawSelf();
}

void OpenGLPainter::prepareForPainting()
{
    glUseProgram(_program.get());

    _mesh.prepareForPainting();
}

void OpenGLPainter::resetAfterPainting()
{
    _mesh.resetAfterPainting();

    glUseProgram(0);
}
