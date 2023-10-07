#include "mesh.h"

//  #   #  #####  #####  #   #
//  ## ##  #      #      #   #
//  # # #  #####  #####  #####
//  #   #  #          #  #   #
//  #   #  #####  #####  #   #

namespace
{
    static GLfloat cubeVertices[]{
        -0.5, -0.5, -0.5, // 0
        -0.5, -0.5, 0.5,  // 1
        -0.5, 0.5, 0.5,   // 2
        -0.5, 0.5, -0.5,  // 3
        0.5, -0.5, -0.5,  // 4
        0.5, -0.5, 0.5,   // 5
        0.5, 0.5, 0.5,    // 6
        0.5, 0.5, -0.5,   // 7
    };

    static GLushort cubeIndices[]{
        0, 1, 3,
        1, 2, 3,
        0, 1, 5,
        0, 4, 5,
        1, 2, 6,
        1, 6, 5,
        2, 6, 3,
        7, 6, 3,
        6, 5, 7,
        4, 5, 7,
        0, 3, 7,
        0, 4, 7};
}

Mesh::Mesh(QOpenGLContext *context)
    : QOpenGLExtraFunctions{context},
      _context{context}
{
    if (_context)
    {
        initializeOpenGLFunctions();

        glGenVertexArrays(1, &_VAO);
        if (_VAO != 0)
        {
            glBindVertexArray(_VAO);
            {
                // for indexed draws we need to generate another buffer that will contain indices
                glGenBuffers(1, &_IBO);
                if (_IBO != 0)
                {
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
                    {
                        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices),
                                     cubeIndices, GL_STATIC_DRAW);
                    }
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

                    glGenBuffers(1, &_VBO);
                    if (_VBO != 0)
                    {
                        _isValid = true;
                        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
                        {
                            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices),
                                         cubeVertices, GL_STATIC_DRAW);

                            glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
                            glEnableVertexAttribArray(0);
                        }
                        glBindBuffer(GL_ARRAY_BUFFER, 0);
                    }
                }
            }
        }
        glBindVertexArray(0);
    }
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_IBO);
    glDeleteBuffers(1, &_VBO);
}

void Mesh::prepareForPainting()
{
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
}

void Mesh::resetAfterPainting()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::drawSelf()
{
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
}
