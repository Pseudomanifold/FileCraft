#ifndef FILECRAFT_RENDERER_HH
#define FILECRAFT_RENDERER_HH

#include <QMatrix4x4>
#include <QVector3D>

#include <QOpenGLShaderProgram>

class Chunk;
class Entity;

class Renderer
{
public:
  Renderer( QOpenGLShaderProgram* shaderProgram );

  void clear();

  void render( const Chunk& chunkk, float x, float y, float z );
  void render( const Entity& entity );

  void setProjectionMatrix( const QMatrix4x4& matrix );

  QVector3D eye() const;
  void setEye( const QVector3D& eye );

  QVector3D direction() const;
  void setDirection( const QVector3D& direction );

  QVector3D up() const;

private:
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
};

#endif
