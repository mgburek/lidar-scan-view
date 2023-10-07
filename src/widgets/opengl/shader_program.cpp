#include "shader_program.h"

// #####  #   #  #####  ####   #####  ####    ####   ####   #####  #####  ####   #####  #   #
// #      #   #  #   #  #   #  #      #   #   #   #  #   #  #   #  #      #   #  #   #  ## ##
// #####  #####  #####  #   #  #####  ####    ####   ####   #   #  #  ##  ####   #####  # # #
//     #  #   #  #   #  #   #  #      #  #    #      #  #   #   #  #   #  #  #   #   #  #   #
// #####  #   #  #   #  ####   #####  #   #   #      #   #  #####  #####  #   #  #   #  #   #

QSet<GLuint> ShaderProgram::_programsCache{};

namespace
{
    GLchar vertex_shader_src[] =
        "#version 330 core                                          \n"
        "layout(location = 0) in vec3 pos;                          \n"
        "out vec4 vertex_color;                                    \n"
        "uniform mat4 model;                                        \n"
        "uniform mat4 projection;                                   \n"
        "uniform mat4 view;                                         \n"
        "void main()                                                \n"
        "{                                                          \n"
        " gl_Position = projection * view * model * vec4(pos, 1.0); \n"
        " vertex_color = vec4(clamp(2 * pos, 0.0, 1.0), 1.0);          \n"
        "}\0                                                        ";

    GLchar fragment_shader_src[] =
        "#version 330 core                                  \n"
        "out vec4 colour;                                   \n"
        "in vec4 vertex_color;                             \n"
        "void main()                                        \n"
        "{                                                  \n"
        " colour = vertex_color;                           \n"
        "}\0                                                ";
}

ShaderProgram::ShaderProgram(QOpenGLContext *context)
    : QOpenGLExtraFunctions{context},
      _context{context}
{
    if (_context)
    {
        initializeOpenGLFunctions();

        if (_programID = glCreateProgram())
        {
            _isValid = createVertexShader();
            _isValid = createFragmentShader();
            _isValid = validateProgram();
        }

        if (_isValid)
            _programsCache.insert(_programID);
    }
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(_programID);

    auto prog = _programsCache.find(_programID);
    if (prog != _programsCache.end())
        _programsCache.erase(prog);
}

const GLuint ShaderProgram::get() const
{
    if (_isValid)
        return _programID;
    else
        return 0;
}

bool ShaderProgram::createVertexShader()
{
    if (GLint shaderID = glCreateShader(GL_VERTEX_SHADER))
    {
        return compileAndAttachShader(shaderID, vertex_shader_src);
    }
    return false;
}

bool ShaderProgram::createFragmentShader()
{
    if (GLint shaderID = glCreateShader(GL_FRAGMENT_SHADER))
    {
        return compileAndAttachShader(shaderID, fragment_shader_src);
    }
    return false;
}

bool ShaderProgram::compileAndAttachShader(GLuint shaderID, GLchar *source)
{
    const GLchar *source_code[1];
    source_code[0] = source;

    GLint code_length = strlen(source);

    glShaderSource(shaderID, 1, source_code, &code_length);
    glCompileShader(shaderID);

    GLint result{0};
    GLchar error_log[600]{0};

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(shaderID, sizeof(error_log), NULL, error_log);
        qDebug() << "Shader compilation error << " << error_log;
        return false;
    }

    glAttachShader(_programID, shaderID);
    glDeleteShader(shaderID);

    return true;
}

bool ShaderProgram::validateProgram()
{
    GLint link_result{0};
    GLchar link_error_log[512]{0};

    glLinkProgram(_programID);
    glGetProgramiv(_programID, GL_LINK_STATUS, &link_result);
    if (!link_result)
    {
        glGetProgramInfoLog(_programID, sizeof(link_error_log), NULL, link_error_log);
        qDebug() << "ERROR: Program linking error << " << link_error_log;
        return false;
    }

    glValidateProgram(_programID);
    glGetProgramiv(_programID, GL_VALIDATE_STATUS, &link_result);
    if (!link_result)
    {
        glGetProgramInfoLog(_programID, sizeof(link_error_log), NULL, link_error_log);
        qDebug() << "ERROR: Program validation error << " << link_error_log;
        return false;
    }
    return true;
}