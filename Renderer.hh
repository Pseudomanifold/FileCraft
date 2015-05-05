#ifndef FILECRAFT_RENDERER_HH
#define FILECRAFT_RENDERER_HH

#include <QMatrix4x4>
#include <QOpenGLShaderProgram>

class Entity;

class Renderer
{
public:
  Renderer( QOpenGLShaderProgram* shaderProgram );

  void render( const Entity& entity );

private:
  QOpenGLShaderProgram* _shaderProgram;

  int _modelMatrixLocation;
  QMatrix4x4 _modelMatrix;
};

#endif
