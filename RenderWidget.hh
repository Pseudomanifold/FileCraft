#ifndef FILECRAFT_RENDER_WIDGET_HH
#define FILECRAFT_RENDER_WIDGET_HH

#include <QMatrix4x4>
#include <QVector3D>

#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

#include "Chunk.hh"
#include "Renderer.hh"

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

  bool isCollision( const QVector3D& position );

  QOpenGLShaderProgram* _shaderProgram;

  QMatrix4x4 _modelMatrix;
  QMatrix4x4 _viewMatrix;
  QMatrix4x4 _projectionMatrix;

  int _modelMatrixLocation;
  int _viewMatrixLocation;
  int _projectionMatrixLocation;

  QVector3D _lightDirection;
  int _lightDirectionLocation;

  QVector3D _eye;
  QVector3D _direction;
  QVector3D _up;

  int _mouseX;
  int _mouseY;

  // FIXME: The renderer is by far not stand-alone or anything. It requires
  // some internal and external setup. Still, it simplifies rendering single
  // entities.
  Renderer _renderer;

  // FIXME: This is not the right place to manage a world of chunks, but at
  // least I can pretend to have a working level implementation that way...
  Chunk _chunks[10][10];
};

#endif
