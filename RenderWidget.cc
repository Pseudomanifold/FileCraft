#include "RenderWidget.hh"

#include "Octahedron.hh"
#include "TerrainGenerator.hh"

#include <cmath>

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>

static const char* vertexShaderSource =
  "#version 130\n"
  "in vec3 position;\n"
  "in vec3 normal;\n"
  "in vec3 colour;\n"
  "\n"
  "out vec3 vertexColour;\n"
  "\n"
  "uniform mat4 modelMatrix;\n"
  "uniform mat4 viewMatrix;\n"
  "uniform mat4 projectionMatrix;\n"
  "\n"
  "uniform vec3 lightDirection;\n"
  "varying float diffuseIntensity;\n"
  "\n"
  "void main()\n"
  "{\n"
  "  gl_Position      = projectionMatrix * viewMatrix * modelMatrix * vec4( position, 1.0 );\n"
  "  diffuseIntensity = dot(normal, lightDirection);\n"
  "  vertexColour     = colour;\n"
  "}\n";

static const char* fragmentShaderSource =
  "#version 130\n"
  "in vec3 vertexColour;\n"
  "\n"
  "out vec4 colour;\n"
  "\n"
  "varying float diffuseIntensity;\n"
  "\n"
  "void main()"
  "{\n"
  "  colour = clamp( diffuseIntensity, 0.2f, 1.0f ) * vec4( vertexColour, 1.0 );\n"
  "}\n";

RenderWidget::RenderWidget( QWidget* parent )
  : QOpenGLWidget( parent )
  , _shaderProgram( new QOpenGLShaderProgram( this ) )
  , _modelMatrixLocation( -1 )
  , _viewMatrixLocation( -1 )
  , _projectionMatrixLocation( -1 )
  , _lightDirection( {1.0,1.0,1.0} )
  , _lightDirectionLocation( -1 )
  , _eye(       { 0  , 10.0, 1   } )
  , _direction( { 0.8, -0.3, 0.45} )
  , _up(        { 0  ,  1.0, 0   } )
  , _mouseX( this->width() / 2 )
  , _mouseY( this->height() / 2 )
  , _renderer( _shaderProgram )
{
  _direction.normalize();

  {
    QPoint globalPosition = mapToGlobal( QPoint( this->width()  / 2,
                                                 this->height() / 2 ) );

    QCursor::setPos( globalPosition );
  }

  this->setCursor( Qt::BlankCursor );
  this->setFocusPolicy( Qt::StrongFocus );
  this->setMouseTracking( true );

  for( int i = 0; i < 10; i++ )
  {
    for( int j = 0; j < 10; j++ )
    {
      if( i == 0 || j == 0 || i == 9 || j == 9 )
        _chunks[i][j] = makePeak();
      else
        _chunks[i][j] = makeRandomWithSimplexNoise();
    }
  }
}

void RenderWidget::initializeGL()
{
  glClearColor( 0.25f, 0.25f, 0.25f, 1.f );
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_CULL_FACE );

  _shaderProgram->addShaderFromSourceCode( QOpenGLShader::Vertex, vertexShaderSource );
  _shaderProgram->addShaderFromSourceCode( QOpenGLShader::Fragment, fragmentShaderSource );

  _shaderProgram->bindAttributeLocation( "position", 0 );
  _shaderProgram->bindAttributeLocation( "normal"  , 1 );
  _shaderProgram->bindAttributeLocation( "colour"  , 2 );

  _shaderProgram->link();
  _shaderProgram->bind();

  _modelMatrixLocation      = _shaderProgram->uniformLocation( "modelMatrix" );
  _viewMatrixLocation       = _shaderProgram->uniformLocation( "viewMatrix" );
  _projectionMatrixLocation = _shaderProgram->uniformLocation( "projectionMatrix" );

  _lightDirectionLocation = _shaderProgram->uniformLocation( "lightDirection" );
  _shaderProgram->setUniformValue( _lightDirectionLocation, _lightDirection );

  qDebug() << _shaderProgram->log();
}

void RenderWidget::paintGL()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  _viewMatrix.setToIdentity();
  _viewMatrix.lookAt( _eye,
                      _eye + _direction,
                      _up );

  qDebug() << "Eye:       " << _eye;
  qDebug() << "Direction: " << _direction;

  _shaderProgram->setUniformValue( _modelMatrixLocation,      _modelMatrix );
  _shaderProgram->setUniformValue( _viewMatrixLocation,       _viewMatrix );
  _shaderProgram->setUniformValue( _projectionMatrixLocation, _projectionMatrix );

  for( unsigned int i = 0; i < 10; i++ )
  {
    for( unsigned int j = 0; j < 10; j++ )
    {
      _modelMatrix.setToIdentity();
      _modelMatrix.translate( i * Chunk::xNum, 0.f, j * Chunk::zNum );

      _shaderProgram->setUniformValue( _modelMatrixLocation, _modelMatrix );

      auto&& vertices = _chunks[i][j].vertices();
      auto&& normals  = _chunks[i][j].normals();
      auto&& colours  = _chunks[i][j].colours();

      _shaderProgram->setAttributeArray( 0, GL_FLOAT, vertices.data(), 3 );
      _shaderProgram->setAttributeArray( 1, GL_FLOAT, normals.data(),  3 );
      _shaderProgram->setAttributeArray( 2, GL_FLOAT, colours.data(),  3 );

      _shaderProgram->enableAttributeArray( 0 );
      _shaderProgram->enableAttributeArray( 1 );
      _shaderProgram->enableAttributeArray( 2 );

      glDrawArrays( GL_QUADS, 0, vertices.size() / 3 );

      _shaderProgram->disableAttributeArray( 2 );
      _shaderProgram->disableAttributeArray( 1 );
      _shaderProgram->disableAttributeArray( 0 );
    }
  }

  Octahedron oct;
  oct.setX( 5.f ); oct.setY( 10.f ); oct.setZ( 5.f );

  _renderer.render( oct );
}

void RenderWidget::resizeGL( int w, int h )
{
  _projectionMatrix.setToIdentity();
  _projectionMatrix.perspective( 45.f, static_cast<GLfloat>( w ) / h, 0.01f, 1000.f );
}

void RenderWidget::keyPressEvent( QKeyEvent* event )
{
  auto direction       = _direction;
  auto strafeDirection = QVector3D::crossProduct( direction, _up );
  auto speed           = 0.1f;
  auto previousEye     = _eye;

  switch( event->key() )
  {
  case Qt::Key_W:
    _eye    += speed*direction;
    this->update();
    break;
  case Qt::Key_S:
    _eye    -= speed*direction;
    this->update();
    break;
  case Qt::Key_A:
    _eye    -= speed*strafeDirection;
    this->update();
    break;
  case Qt::Key_D:
    _eye    += speed*strafeDirection;
    this->update();
    break;

  default:
    break;
  }

  if( isCollision( _eye ) )
    _eye = previousEye;
}

void RenderWidget::mouseMoveEvent( QMouseEvent* event )
{
  auto x = event->x();
  auto y = event->y();

  auto xDiff = x - _mouseX;
  auto yDiff = y - _mouseY;

  auto theta = std::acos ( _direction.y() / _direction.length() );
  auto phi   = std::atan2( _direction.z(), _direction.x() );
  auto speed = 0.001f;

  phi   += speed*xDiff;
  theta += speed*yDiff;

  if( theta > 0.75*M_PI)
    theta = 0.75*M_PI;
  else if( theta < 0.25*M_PI )
    theta = 0.25*M_PI;

  _direction.setX( std::sin( theta ) * std::cos( phi ) );
  _direction.setY( std::cos( theta ) );
  _direction.setZ( std::sin( theta ) * std::sin( phi ) );

  _mouseX = this->width() / 2;
  _mouseY = this->height() / 2;

  this->update();

  QPoint globalPosition = mapToGlobal( QPoint( this->width()  / 2,
                                               this->height() / 2 ) );

  QCursor::setPos( globalPosition );
}

bool RenderWidget::isCollision( const QVector3D& position )
{
  // FIXME: Check the player's position. This should become a method in a
  // "World" class or somewhere else.

  int x = static_cast<int>( std::floor( position.x() ) );
  int y = static_cast<int>( std::floor( position.y() ) );
  int z = static_cast<int>( std::floor( position.z() + 1.f ) );

  if( x >= 0 && x < 10*Chunk::xNum && z >= 0 && z < 10*Chunk::zNum )
  {
    // Global chunk that is next to our position
    int xChunk = x / Chunk::xNum;
    int yChunk = z / Chunk::zNum;

    // Translate coordinates into local chunk system
    int xLocal = x - xChunk * Chunk::xNum;
    int yLocal = y;
    int zLocal = z - yChunk * Chunk::zNum;

    qDebug() << "Global chunk        :" << xChunk << yChunk;
    qDebug() << "Local chunk position:" << xLocal << "," << yLocal << "," << zLocal;

    if( _chunks[xChunk][yChunk].isOccupied( xLocal, yLocal, zLocal ) )
      return true;
  }

  return false;
}
