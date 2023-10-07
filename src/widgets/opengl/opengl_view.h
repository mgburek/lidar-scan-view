#ifndef OPENGL_VIEW_H
#define OPENGL_VIEW_H

#include <QOpenGLWidget>
#include <QVector3D>

#include "opengl_painter.h"
#include "camera.h"
#include "scan_data_model.h"

// #####  ####   #####  #   #  #####  #       #   #  #  #####  #   #
// #   #  #   #  #      ##  #  #      #       #   #  #  #      # # #
// #   #  ####   #####  # # #  #  ##  #       #   #  #  #####  # # #
// #   #  #      #      #  ##  #   #  #        # #   #  #      # # #
// #####  #      #####  #   #  #####  #####     #    #  #####   # #

class OpenGLView : public QOpenGLWidget
{
public:
    OpenGLView(QWidget *parent, ScanDataModel *scanDataModel);
    ~OpenGLView();

private:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;

private:
    ScanDataModel *_scanDataModel;
    OpenGLPainter *_painter;
    Camera *_camera;

    QPointF _oldMousePos;
};

#endif // OPENGL_VIEW_H