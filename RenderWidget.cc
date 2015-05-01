#include "RenderWidget.hh"

RenderWidget::RenderWidget( QWidget* parent )
  : QOpenGLWidget( parent )
{
}

void RenderWidget::initializeGL()
{
  glClearColor( 0.25f, 0.25f, 0.25f, 1.f );
}

void RenderWidget::paintGL()
{
}
