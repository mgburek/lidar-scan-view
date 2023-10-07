#include "opengl_view.h"

#include <QKeyEvent>
#include <QWheelEvent>

// #####  ####   #####  #   #  #####  #       #   #  #  #####  #   #
// #   #  #   #  #      ##  #  #      #       #   #  #  #      # # #
// #   #  ####   #####  # # #  #  ##  #       #   #  #  #####  # # #
// #   #  #      #      #  ##  #   #  #        # #   #  #      # # #
// #####  #      #####  #   #  #####  #####     #    #  #####   # #

OpenGLView::OpenGLView(QWidget *parent, ScanDataModel *scanDataModel)
    : QOpenGLWidget(parent),
      _scanDataModel{scanDataModel},
      _painter{nullptr},
      _camera{nullptr}
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setMinimumSize(600, 400);

    grabGesture(Qt::PanGesture);
}

OpenGLView::~OpenGLView()
{
    delete _painter;
    delete _camera;
}

void OpenGLView::initializeGL()
{
    _painter = new OpenGLPainter(this->context());
    _camera = new Camera(this->context());

    QOpenGLWidget::initializeGL();
}

void OpenGLView::resizeGL(int w, int h)
{
    _camera->setAspectRatio(float(w) / float(h));
    QOpenGLWidget::resizeGL(w, h);
}

void OpenGLView::paintGL()
{
    _painter->prepareForPainting();
    {
        for (const Point &p : _scanDataModel->points())
        {
            _painter->paintMesh(p.x(), p.y(), p.z(), p.scale());
        }

        _painter->paintMesh(0, 0, 0);
    }
    _painter->resetAfterPainting();
    QOpenGLWidget::paintGL();
}

void OpenGLView::keyPressEvent(QKeyEvent *event)
{
    switch (event->keyCombination())
    {
    case QKeyCombination(Qt::Key_Right):
        _camera->yaw(-5.0f);
        break;
    case QKeyCombination(Qt::Key_Left):
        _camera->yaw(5.0f);
        break;
    case QKeyCombination(Qt::Key_Down):
        _camera->pitch(-5.0f);
        break;
    case QKeyCombination(Qt::Key_Up):
        _camera->pitch(5.0f);
        break;
    case QKeyCombination(Qt::ShiftModifier, Qt::Key_Up):
        _camera->zoomIn(-0.5f);
        break;
    case QKeyCombination(Qt::ShiftModifier, Qt::Key_Down):
        _camera->zoomIn(0.5f);
        break;
    default:
        break;
    }
    QOpenGLWidget::update();
}

void OpenGLView::wheelEvent(QWheelEvent *event)
{
    QPoint angle = event->angleDelta() / 50.0;
    _camera->zoomIn(-angle.y());

    QOpenGLWidget::update();
}

void OpenGLView::mousePressEvent(QMouseEvent *event)
{
    _oldMousePos = event->position();
}

void OpenGLView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton || event->buttons() == Qt::RightButton)
        event->accept();
    else
        event->ignore();

    if (event->isAccepted())
    {
        QPointF pos = event->position();

        QPointF deltaPos = pos - _oldMousePos;

        switch (event->buttons())
        {
        case Qt::LeftButton:
            _camera->yaw(deltaPos.x() * 0.3);
            _camera->pitch(deltaPos.y() * 0.3);
            break;
        case Qt::RightButton:
            _camera->pan(deltaPos.x() * 0.05, deltaPos.y() * 0.05);
            break;
        }

        QOpenGLWidget::update();
        _oldMousePos = pos;
    }
}