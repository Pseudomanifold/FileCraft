#ifndef FILECRAFT_RENDER_WIDGET_HH
#define FILECRAFT_RENDER_WIDGET_HH

#include <QMatrix4x4>
#include <QVector3D>

#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

class RenderWidget : public QOpenGLWidget
{
  Q_OBJECT

public:
  RenderWidget( QWidget* parent = nullptr );

private:
  virtual void initializeGL();
  virtual void paintGL();
  virtual void resizeGL( int w, int h );

  virtual void keyPressEvent( QKeyEvent* event );
  virtual void mouseMoveEvent( QMouseEvent* event );

  QOpenGLShaderProgram* _shaderProgram;

  QMatrix4x4 _modelViewMatrix;
  QMatrix4x4 _projectionMatrix;

  int _modelViewMatrixLocation;
  int _projectionMatrixLocation;

  QVector3D _lightDirection;
  int _lightDirectionLocation;

  QVector3D _eye;
  QVector3D _centre;
  QVector3D _up;

  int _mouseX;
  int _mouseY;
};

#endif
