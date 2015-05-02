#include "RenderWidget.hh"
#include "Chunk.hh"

#include <QDebug>

static const char* vertexShaderSource =
  "#version 130\n"
  "in vec3 position;\n"
  "\n"
  "uniform mat4 modelViewMatrix;\n"
  "uniform mat4 projectionMatrix;\n"
  "\n"
  "void main()\n"
  "{\n"
  "  gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );\n"
  "}\n";

static const char* fragmentShaderSource =
  "#version 130\n"
  "out vec4 colour;\n"
  "\n"
  "void main()"
  "{\n"
  "  colour = vec4( 1.0, 1.0, 1.0, 1.0 );\n"
  "}\n";

RenderWidget::RenderWidget( QWidget* parent )
  : QOpenGLWidget( parent )
  , _shaderProgram( new QOpenGLShaderProgram( this ) )
{
}

void RenderWidget::initializeGL()
{
  glClearColor( 0.25f, 0.25f, 0.25f, 1.f );

  _shaderProgram->addShaderFromSourceCode( QOpenGLShader::Vertex, vertexShaderSource );
  _shaderProgram->addShaderFromSourceCode( QOpenGLShader::Fragment, fragmentShaderSource );

  _shaderProgram->bindAttributeLocation( "position", 0 );

  _shaderProgram->link();
  _shaderProgram->bind();

  _modelViewMatrixLocation  = _shaderProgram->uniformLocation( "modelViewMatrix" );
  _projectionMatrixLocation = _shaderProgram->uniformLocation( "projectionMatrix" );

  qDebug() << _shaderProgram->log();
}

void RenderWidget::paintGL()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  _modelViewMatrix.setToIdentity();
  _modelViewMatrix.lookAt( QVector3D( 10,  2, -5 ),
                           QVector3D(  5,  0,  0 ),
                           QVector3D(  0,  1,  0 ) );

  _shaderProgram->setUniformValue( _modelViewMatrixLocation, _modelViewMatrix );
  _shaderProgram->setUniformValue( _projectionMatrixLocation, _projectionMatrix );

  Chunk chunk;
  auto vertices = chunk.vertices();

  _shaderProgram->setAttributeArray( 0, GL_FLOAT, vertices.data(), 3 );
  _shaderProgram->enableAttributeArray( 0 );

  glDrawArrays( GL_QUADS, 0, vertices.size() / 3 );

  _shaderProgram->disableAttributeArray( 0 );

  qDebug() << _shaderProgram->log();
}

void RenderWidget::resizeGL( int w, int h )
{
  _projectionMatrix.setToIdentity();
  _projectionMatrix.perspective( 45.f, static_cast<GLfloat>( w ) / h, 0.01f, 1000.f );
}
