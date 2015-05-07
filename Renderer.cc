#include "Renderer.hh"

#include "Entity.hh"

Renderer::Renderer( QOpenGLShaderProgram* shaderProgram )
  : _shaderProgram( shaderProgram )
{
}

void Renderer::render( const Entity& entity )
{
  // FIXME: This is stupid and should not be required. However, I need to
  // decide where to bind and link the shader program eventually. Should this
  // burden lie on the renderer?
  _modelMatrixLocation = _shaderProgram->uniformLocation( "modelMatrix" );

  _modelMatrix.setToIdentity();
  _modelMatrix.translate( entity.x(), entity.y(), entity.z() );
  _shaderProgram->setUniformValue( _modelMatrixLocation, _modelMatrix );

  auto&& vertices = entity.vertices();
  auto&& normals  = entity.normals();
  auto&& colours  = entity.colours();

  _shaderProgram->setAttributeArray( 0, GL_FLOAT, vertices.data(), 3 );
  _shaderProgram->setAttributeArray( 1, GL_FLOAT, normals.data(),  3 );
  _shaderProgram->setAttributeArray( 2, GL_FLOAT, colours.data(),  3 );

  _shaderProgram->enableAttributeArray( 0 );
  _shaderProgram->enableAttributeArray( 1 );
  _shaderProgram->enableAttributeArray( 2 );

  glDrawArrays( GL_TRIANGLES, 0, vertices.size() / 3 );

  _shaderProgram->disableAttributeArray( 2 );
  _shaderProgram->disableAttributeArray( 1 );
  _shaderProgram->disableAttributeArray( 0 );
}
