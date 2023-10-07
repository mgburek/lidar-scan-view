#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <QOpenGLExtraFunctions>

// #####  #   #   ###   ####   #####  ####      ####   ####   #####  #####  ####    ###   #   #
// #      #   #  #   #  #   #  #      #   #     #   #  #   #  #   #  #      #   #  #   #  ## ##
// #####  #####  #####  #   #  #####  ####      ####   ####   #   #  #  ##  ####   #####  # # #
//     #  #   #  #   #  #   #  #      #  #      #      #  #   #   #  #   #  #  #   #   #  #   #
// #####  #   #  #   #  #####  #####  #   #     #      #   #  #####  #####  #   #  #   #  #   #

class ShaderProgram : QOpenGLExtraFunctions
{
public:
    inline static const QSet<GLuint> getPrograms() { return _programsCache; }

    ShaderProgram(QOpenGLContext *context);
    ~ShaderProgram();

    const GLuint get() const;

private:
    bool createVertexShader();
    bool createFragmentShader();
    bool compileAndAttachShader(GLuint shaderID, GLchar *source);
    bool validateProgram();

private:
    QOpenGLContext *_context;
    GLuint _programID{0};
    bool _isValid{false};

    static QSet<GLuint> _programsCache;
};

#endif // SHADER_PROGRAM_H