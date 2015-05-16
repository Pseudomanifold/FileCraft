#include "RenderWidget.hh"

#include "BoundingBox.hh"
#include "Octahedron.hh"
#include "Renderer.hh"
#include "TerrainGenerator.hh"

#include <cmath>

#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>

RenderWidget::RenderWidget( QWidget* parent )
  : QOpenGLWidget( parent )
  , _shaderProgram( new QOpenGLShaderProgram( this ) )
  , _mouseX( this->width() / 2 )
  , _mouseY( this->height() / 2 )
{
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
      _chunks[i][j] = makeRandomWithSimplexNoise(i*10.f*Chunk::xNum, 0.f, j*10.f*Chunk::zNum);
    }
  }
}

void RenderWidget::initializeGL()
{
  _renderer = new Renderer( _shaderProgram );
}

void RenderWidget::paintGL()
{
  _renderer->clear();

  for( int i = 0; i < 10; i++ )
    for( int j = 0; j < 10; j++ )
      _renderer->render( _chunks[i][j], i * Chunk::xNum, 0.f, j * Chunk::zNum );

  Octahedron oct;
  oct.setX( 5.f ); oct.setY( 10.f ); oct.setZ( 5.f );

  auto eye = _renderer->eye();

  BoundingBox bb1( 4, 6, 9, 11, 4, 6 );
  BoundingBox bb2( eye.x() - 0.5*1.41, eye.x() + 0.5*1.41,
                   eye.y() - 0.5*1.41, eye.y() + 0.5*1.41,
                   eye.z() - 0.5*1.41, eye.z() + 0.5*1.41 );

  if( bb1.intersects( bb2 ) )
    qDebug() << "Intersection with octahedron";

  _renderer->render( oct );
}

void RenderWidget::resizeGL( int w, int h )
{
  QMatrix4x4 projectionMatrix;
  projectionMatrix.perspective( 45.f, static_cast<GLfloat>( w ) / h, 0.01f, 1000.f );

  _renderer->setProjectionMatrix( projectionMatrix );
}

void RenderWidget::keyPressEvent( QKeyEvent* event )
{
  auto direction       = _renderer->direction();
  auto strafeDirection = QVector3D::crossProduct( direction, _renderer->up() );
  auto speed           = 0.1f;
  auto previousEye     = _renderer->eye();

  QVector3D deltaDirection;

  switch( event->key() )
  {
  case Qt::Key_W:
    deltaDirection =  direction;
    break;
  case Qt::Key_S:
    deltaDirection = -direction;;
    break;
  case Qt::Key_A:
    deltaDirection = -strafeDirection;
    break;
  case Qt::Key_D:
    deltaDirection =  strafeDirection;
    break;

  default:
    break;
  }

  auto xEye = previousEye;
  auto yEye = previousEye;
  auto zEye = previousEye;

  auto eye          = _renderer->eye();
  auto testPosition = eye + 3 * speed * deltaDirection;

  xEye.setX( testPosition.x() );
  yEye.setY( testPosition.y() );
  zEye.setZ( testPosition.z() );

  eye += speed * deltaDirection;

  if( isCollision( xEye ) )
    eye.setX(  previousEye.x() );

  if( isCollision( yEye ) )
    eye.setY( previousEye.y() );

  if( isCollision( zEye ) )
    eye.setZ( previousEye.z() );

  _renderer->setEye( eye );

  this->update();
}

void RenderWidget::mouseMoveEvent( QMouseEvent* event )
{
  auto x = event->x();
  auto y = event->y();

  auto xDiff = x - _mouseX;
  auto yDiff = y - _mouseY;

  auto direction = _renderer->direction();
  auto theta     = std::acos ( direction.y() / direction.length() );
  auto phi       = std::atan2( direction.z(), direction.x() );
  auto speed     = 0.001f;

  phi   += speed*xDiff;
  theta += speed*yDiff;

  if( theta > 0.75*M_PI)
    theta = 0.75*M_PI;
  else if( theta < 0.25*M_PI )
    theta = 0.25*M_PI;

  direction.setX( std::sin( theta ) * std::cos( phi ) );
  direction.setY( std::cos( theta ) );
  direction.setZ( std::sin( theta ) * std::sin( phi ) );

  _mouseX = this->width() / 2;
  _mouseY = this->height() / 2;

  _renderer->setDirection( direction );

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
