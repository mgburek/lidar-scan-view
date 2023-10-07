#include "camera.h"

#include "shader_program.h"

//  #####  #####  #   #  #####  ####   #####
//  #      #   #  ## ##  #      #   #  #   #
//  #      #####  # # #  #####  ####   #####
//  #      #   #  #   #  #      #  #   #   #
//  #####  #   #  #   #  #####  #   #  #   #

Camera::Camera(QOpenGLContext *context)
    : QOpenGLExtraFunctions(context),
      _context{context}
{
    if (_context)
    {
        initializeOpenGLFunctions();
        update();
    }
}

void Camera::update()
{
    QMatrix4x4 projectionMatrix = QMatrix4x4();
    projectionMatrix.perspective(_verticalAngle, _aspectRatio, _nearPlane, _farPlane);

    QMatrix4x4 viewMatrix = QMatrix4x4();
    viewMatrix.lookAt(_cameraPosition, _cameraTarget, _cameraUp);
    viewMatrix.translate(_viewTranslation);
    viewMatrix.scale(_viewScale);

    for (GLuint program : ShaderProgram::getPrograms())
    {
        glUseProgram(program);
        {
            GLuint attr_id = glGetUniformLocation(program, "view");
            glUniformMatrix4fv(attr_id, 1, GL_FALSE, viewMatrix.constData());

            attr_id = glGetUniformLocation(program, "projection");
            glUniformMatrix4fv(attr_id, 1, GL_FALSE, projectionMatrix.constData());
        }
    }
    glUseProgram(0);
}

void Camera::setAspectRatio(float ratio)
{
    _aspectRatio = ratio;
    update();
}

void Camera::zoomIn(float deltaDistance)
{
    float currentDistance = _cameraTarget.distanceToPoint(_cameraPosition);

    currentDistance += deltaDistance;
    if (currentDistance < 1.0)
        currentDistance = 1.0;

    _cameraPosition = _cameraPosition.normalized() * currentDistance;

    update();
}

void Camera::pitch(float deltaAngle)
{
    float currentAngle =
        std::asin(QVector3D::dotProduct(_cameraUp, _cameraPosition) /
                  (_cameraUp.length() * _cameraPosition.length()));
    currentAngle = (currentAngle / M_PI) * 180.0f; 

    const static float maxPitchAngle{89.9f};
    const static float minPitchAngle{-89.9f};

    if (currentAngle - deltaAngle >= maxPitchAngle)
        deltaAngle = currentAngle - maxPitchAngle;
    else if (currentAngle - deltaAngle <= minPitchAngle)
        deltaAngle = currentAngle - minPitchAngle;

    QVector3D cameraDirection = _cameraTarget - _cameraPosition;
    QVector3D rotationAxis = QVector3D::crossProduct(cameraDirection, _cameraUp);

    QMatrix4x4 trafo;
    trafo.rotate(deltaAngle, rotationAxis);

    QVector4D transformed = trafo * QVector4D(_cameraPosition);

    _cameraPosition = transformed.toVector3D();

    update();
}

void Camera::yaw(float deltaAngle)
{
    QVector4D vec4_cameraPosition(_cameraPosition);

    QMatrix4x4 trafo;
    trafo.rotate(deltaAngle, _cameraUp);

    QVector4D transformed = trafo * vec4_cameraPosition;

    _cameraPosition = transformed.toVector3D();

    update();
}

void Camera::pan(float x, float y)
{
    QVector3D cameraDirection = _cameraTarget - _cameraPosition;
    QVector3D cameraXAxis = QVector3D::crossProduct(cameraDirection, _cameraUp);
    QVector3D cameraYAxis = QVector3D::crossProduct(cameraDirection, cameraXAxis);

    _viewTranslation += cameraXAxis.normalized() * x;
    _viewTranslation += cameraYAxis.normalized() * y;
    update();
}