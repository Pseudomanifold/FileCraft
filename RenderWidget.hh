#ifndef FILECRAFT_RENDER_WIDGET_HH
#define FILECRAFT_RENDER_WIDGET_HH

#include <QMatrix4x4>

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

  QOpenGLShaderProgram* _shaderProgram;

  QMatrix4x4 _modelViewMatrix;
  QMatrix4x4 _projectionMatrix;

  int _modelViewMatrixLocation;
  int _projectionMatrixLocation;
};

#endif
