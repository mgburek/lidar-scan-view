#ifndef MESH_H
#define MESH_H

#include <QOpenGLExtraFunctions>

//  #   #  #####  #####  #   #
//  ## ##  #      #      #   #
//  # # #  #####  #####  #####
//  #   #  #          #  #   #
//  #   #  #####  #####  #   #

class Mesh : QOpenGLExtraFunctions
{
public:
    Mesh(QOpenGLContext *context);
    ~Mesh();

    void prepareForPainting();
    void resetAfterPainting();
    void drawSelf();

private:
    QOpenGLContext *_context;

    bool _isValid{false};

    GLuint _VAO{0};
    GLuint _VBO{0};
    GLuint _IBO{0};
};

#endif // MESH_H