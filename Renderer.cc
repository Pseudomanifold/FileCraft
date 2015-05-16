#include "Renderer.hh"

#include "Chunk.hh"
#include "Entity.hh"

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

Renderer::Renderer( QOpenGLShaderProgram* shaderProgram )
  : _shaderProgram( shaderProgram )
  , _lightDirection( {1.0,1.0,1.0} )
  , _eye(       { 0  , 10.0, 1   } )
  , _direction( { 0.8, -0.3, 0.45} )
  , _up(        { 0  ,  1.0, 0   } )

{
  glClearColor( 1.f, 1.f, 1.f, 1.f );
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
}

void Renderer::clear()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  _viewMatrix.setToIdentity();
  _viewMatrix.lookAt( _eye,
                      _eye + _direction,
                      _up );

  _shaderProgram->setUniformValue( _modelMatrixLocation,      _modelMatrix );
  _shaderProgram->setUniformValue( _viewMatrixLocation,       _viewMatrix );
  _shaderProgram->setUniformValue( _projectionMatrixLocation, _projectionMatrix );
}

void Renderer::render( const Chunk& chunk, float x, float y, float z )
{
  _modelMatrix.setToIdentity();
  _modelMatrix.translate( x, y, z );
  _shaderProgram->setUniformValue( _modelMatrixLocation, _modelMatrix );

  auto&& vertices = chunk.vertices();
  auto&& normals  = chunk.normals();
  auto&& colours  = chunk.colours();

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

void Renderer::render( const Entity& entity )
{
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

void Renderer::setProjectionMatrix( const QMatrix4x4& matrix )
{
  _projectionMatrix = matrix;
}
