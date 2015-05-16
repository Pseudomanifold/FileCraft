#ifndef FILECRAFT_RENDER_WIDGET_HH
#define FILECRAFT_RENDER_WIDGET_HH

#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

#include "Chunk.hh"

class Renderer;

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

  int _mouseX;
  int _mouseY;

  // FIXME: The renderer is by far not stand-alone or anything. It requires
  // some internal and external setup. Still, it simplifies rendering single
  // entities.
  Renderer* _renderer;

  // FIXME: This is not the right place to manage a world of chunks, but at
  // least I can pretend to have a working level implementation that way...
  Chunk _chunks[10][10];
};

#endif
