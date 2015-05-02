#ifndef FILECRAFT_RENDER_WIDGET_HH
#define FILECRAFT_RENDER_WIDGET_HH

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

  QOpenGLShaderProgram* _shaderProgram;
};

#endif
