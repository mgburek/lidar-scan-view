#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLExtraFunctions>

#include <QMatrix4x4>
#include <QVector3D>

//  #####  #####  #   #  #####  ####   #####
//  #      #   #  ## ##  #      #   #  #   #
//  #      #####  # # #  #####  ####   #####
//  #      #   #  #   #  #      #  #   #   #
//  #####  #   #  #   #  #####  #   #  #   #

class Camera : QOpenGLExtraFunctions
{
public:
    Camera(QOpenGLContext *context);

    void setAspectRatio(float ratio);
    void zoomIn(float deltaDistance);
    void pitch(float deltaAngle);
    void yaw(float deltaAngle);
    void pan(float x, float y);

private:
    void update();

private:
    QOpenGLContext *_context;

    QVector3D _cameraPosition{1.0f, 1.0f, 1.0f};
    QVector3D _cameraTarget{0.0f, 0.0f, 0.0f};
    QVector3D _cameraUp{0.0f, 1.0f, 0.0f};

    QVector3D _viewTranslation{0.0f, 0.0f, 0.0f};

    QMatrix4x4 _projectionMatrix;
    float _aspectRatio{1.0};

    inline static const float _nearPlane{0.1};
    inline static const float _farPlane{80.0};
    inline static const float _verticalAngle{45.0};

    inline static const float _viewScale{0.05f};
};

#endif // CAMERA_H