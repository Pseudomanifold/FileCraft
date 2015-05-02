#include "RenderWidget.hh"
#include "Chunk.hh"

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>

static const char* vertexShaderSource =
  "#version 130\n"
  "in vec3 position;\n"
  "in vec3 normal;\n"
  "\n"
  "uniform mat4 modelViewMatrix;\n"
  "uniform mat4 projectionMatrix;\n"
  "\n"
  "uniform vec3 lightDirection;\n"
  "varying float diffuseIntensity;\n"
  "\n"
  "void main()\n"
  "{\n"
  "  gl_Position      = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );\n"
  "  diffuseIntensity = dot(normal, lightDirection);\n"
  "}\n";

static const char* fragmentShaderSource =
  "#version 130\n"
  "out vec4 colour;\n"
  "\n"
  "varying float diffuseIntensity;\n"
  "\n"
  "void main()"
  "{\n"
  "  colour = clamp( diffuseIntensity, 0.2f, 1.0f ) * vec4( 1.0, 1.0, 1.0, 1.0 );\n"
  "}\n";

RenderWidget::RenderWidget( QWidget* parent )
  : QOpenGLWidget( parent )
  , _shaderProgram( new QOpenGLShaderProgram( this ) )
  , _modelViewMatrixLocation( -1 )
  , _projectionMatrixLocation( -1 )
  , _lightDirection( {1,1,1} )
  , _lightDirectionLocation( -1 )
  , _eye(    {10,2,-5} )
  , _centre( { 5,0, 0} )
  , _up(     { 0,1, 0} )
  , _mouseX( 0 )
  , _mouseY( 0 )
{
  this->setFocusPolicy( Qt::StrongFocus );
  this->setMouseTracking( true );
}

void RenderWidget::initializeGL()
{
  glClearColor( 0.25f, 0.25f, 0.25f, 1.f );
  glEnable( GL_DEPTH_TEST );

  _shaderProgram->addShaderFromSourceCode( QOpenGLShader::Vertex, vertexShaderSource );
  _shaderProgram->addShaderFromSourceCode( QOpenGLShader::Fragment, fragmentShaderSource );

  _shaderProgram->bindAttributeLocation( "position", 0 );
  _shaderProgram->bindAttributeLocation( "normal"  , 1 );

  _shaderProgram->link();
  _shaderProgram->bind();

  _modelViewMatrixLocation  = _shaderProgram->uniformLocation( "modelViewMatrix" );
  _projectionMatrixLocation = _shaderProgram->uniformLocation( "projectionMatrix" );

  _lightDirectionLocation = _shaderProgram->uniformLocation( "lightDirection" );
  _shaderProgram->setUniformValue( _lightDirectionLocation, _lightDirection );

  qDebug() << _shaderProgram->log();
}

void RenderWidget::paintGL()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  _modelViewMatrix.setToIdentity();
  _modelViewMatrix.lookAt( _eye,
                           _centre,
                           _up );

  _shaderProgram->setUniformValue( _modelViewMatrixLocation, _modelViewMatrix );
  _shaderProgram->setUniformValue( _projectionMatrixLocation, _projectionMatrix );

  Chunk chunk;
  auto vertices = chunk.vertices();
  auto normals  = chunk.normals();

  _shaderProgram->setAttributeArray( 0, GL_FLOAT, vertices.data(), 3 );
  _shaderProgram->setAttributeArray( 1, GL_FLOAT, normals.data(),  3 );

  _shaderProgram->enableAttributeArray( 0 );
  _shaderProgram->enableAttributeArray( 1 );

  glDrawArrays( GL_QUADS, 0, vertices.size() / 3 );

  _shaderProgram->disableAttributeArray( 0 );
  _shaderProgram->disableAttributeArray( 1 );


  qDebug() << _shaderProgram->log();
}

void RenderWidget::resizeGL( int w, int h )
{
  _projectionMatrix.setToIdentity();
  _projectionMatrix.perspective( 45.f, static_cast<GLfloat>( w ) / h, 0.01f, 1000.f );
}

void RenderWidget::keyPressEvent( QKeyEvent* event )
{
  auto direction       = _centre - _eye;
  auto strafeDirection = QVector3D::crossProduct( direction, _up );
  auto speed           = 0.1f;

  switch( event->key() )
  {
  case Qt::Key_W:
    qDebug() << "Forward";
    _eye    += speed*direction;
    _centre += speed*direction;

    this->update();
    break;
  case Qt::Key_S:
    qDebug() << "Backward";
    _eye    -= speed*direction;
    _centre -= speed*direction;

    this->update();
    break;
  case Qt::Key_A:
    qDebug() << "Strafe left";
    _eye    -= speed*strafeDirection;
    _centre -= speed*strafeDirection;

    this->update();
    break;

  case Qt::Key_D:
    qDebug() << "Strafe right";
    _eye    += speed*strafeDirection;
    _centre += speed*strafeDirection;

    this->update();
    break;
  default:
    break;
  }
}

void RenderWidget::mouseMoveEvent( QMouseEvent* event )
{
  auto x = event->x();
  auto y = event->y();

  auto xDiff = x - _mouseX;
  auto yDiff = y - _mouseY;

  _centre.setX( _centre.x() - 0.01f*xDiff );
  _centre.setY( _centre.y() - 0.01f*yDiff );

  _mouseX = this->width() / 2;
  _mouseY = this->height() / 2;

  this->update();

  QPoint globalPosition = mapToGlobal( QPoint( this->width()  / 2,
                                               this->height() / 2 ) );

  QCursor::setPos( globalPosition );
}
