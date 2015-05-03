#include "Chunk.hh"

#include <array>
#include <stdexcept>

#include <cmath>

namespace
{

std::array<GLfloat,24*3> getSubChunkVertices( GLfloat x, GLfloat y, GLfloat z )
{
  std::array<GLfloat, 24*3> vertices
    = {
       {
        x  , y  , z  , // front
        x+1, y  , z  ,
        x+1, y+1, z  ,
        x  , y+1, z  ,
        x,   y  , z-1, // back
        x,   y+1, z-1,
        x+1, y+1, z-1,
        x+1, y  , z-1,
        x,   y  , z  , // left
        x,   y+1, z  ,
        x,   y+1, z-1,
        x,   y  , z-1,
        x+1, y,   z  , // right
        x+1, y,   z-1,
        x+1, y+1, z-1,
        x+1, y+1, z  ,
        x,   y+1, z  , // top
        x+1, y+1, z  ,
        x+1, y+1, z-1,
        x  , y+1, z-1,
        x,   y,   z  , // bottom
        x,   y,   z-1,
        x+1, y,   z-1,
        x+1, y,   z
       }
  };

  return vertices;
}

std::array<GLfloat,24*3> getSubChunkNormals()
{
  std::array<GLfloat, 24*3> normals
    = {
       {
         0, 0, 1, // front
         0, 0, 1,
         0, 0, 1,
         0, 0, 1,
         0, 0,-1, // back
         0, 0,-1,
         0, 0,-1,
         0, 0,-1,
        -1, 0, 0, // left
        -1, 0, 0,
        -1, 0, 0,
        -1, 0, 0,
         1, 0, 0, // right
         1, 0, 0,
         1, 0, 0,
         1, 0, 0,
         0, 1, 0, // top
         0, 1, 0,
         0, 1, 0,
         0, 1, 0,
         0,-1, 0, // bottom
         0,-1, 0,
         0,-1, 0,
         0,-1, 0
       }
  };

  return normals;
}

std::array<GLfloat,24*3> getSubChunkColours( unsigned int type )
{
  std::array<GLfloat, 24*3> colours;

  GLfloat r = 0.f;
  GLfloat g = 0.f;
  GLfloat b = 0.f;

  switch( type )
  {
  // green terrain
  case 1:
    r = 0.05f; g = 0.50f; b = 0.05f;
    break;

  // brown terrain
  case 2:
    r = 0.75f; g = 0.25f; b = 0.05f;
    break;

  // blue terrain
  case 3:
    r = 0.30f; g = 0.40f; b = 1.00f;

  default:
      break;
  }

  for( unsigned int i = 0; i < 24; i += 3 )
  {
    colours[i  ] = r;
    colours[i+1] = g;
    colours[i+2] = b;
  }

  return colours;
}

} // end of anonymous namespace

Chunk::Chunk()
  : _updateRequired( true )
{
  for( data_type x = 0; x < xNum; x++ )
    for( data_type y = 0; y < yNum; y++ )
      for( data_type z = 0; z < zNum; z++ )
        _data[x][y][z] = 0;
}

const Chunk::data_type& Chunk::operator()( unsigned int x, unsigned int y, unsigned int z ) const
{
  if( x >= xNum || y >= yNum || z >= zNum )
    throw std::runtime_error( "Attempting to access invalid index" );

  return _data[x][y][z];
}

Chunk::data_type& Chunk::operator()( unsigned int x, unsigned int y, unsigned int z )
{
  return const_cast<Chunk::data_type&>( static_cast<const Chunk&>( *this ).operator()( x, y, z ) );
}

const std::vector<GLfloat>& Chunk::vertices() const
{
  if( _updateRequired )
    this->update();

  return _vertices;
}

const std::vector<GLfloat>& Chunk::normals() const
{
  if( _updateRequired )
    this->update();

  return _normals;
}

const std::vector<GLfloat>& Chunk::colours() const
{
  if( _updateRequired )
    this->update();

  return _colours;
}

bool Chunk::isOccupied( int x, int y, int z ) const
{
  if( x < 0 || x >= xNum )
    return false;

  if( y < 0 || y >= yNum )
    return false;

  if( z < 0 || z >= zNum )
    return false;

  return _data[x][y][z] != 0;
}

void Chunk::update() const
{
  _vertices.clear();
  _vertices.reserve( xNum * yNum * zNum * 3 );

  _normals.clear();
  _normals.reserve( xNum * yNum * zNum * 3 );

  _colours.clear();
  _colours.reserve( xNum * yNum * zNum * 3 );

  for( data_type x = 0; x < xNum; x++ )
  {
    for( data_type y = 0; y < yNum; y++ )
    {
      for( data_type z = 0; z < zNum; z++ )
      {
        if( _data[x][y][z] != 0 )
        {
          auto&& subChunkVertices = getSubChunkVertices( x, y, z );

          _vertices.insert( std::end( _vertices ),
                            std::begin( subChunkVertices ), std::end( subChunkVertices ) );

          auto&& subChunkNormals = getSubChunkNormals();

          _normals.insert( std::end( _normals ),
                           std::begin( subChunkNormals ), std::end( subChunkNormals ) );

          auto&& subChunkColours = getSubChunkColours( _data[x][y][z] );

          _colours.insert( std::end( _colours ),
                           std::begin( subChunkColours ), std::end( subChunkColours ) );
        }
      }
    }
  }

  _updateRequired = false;
}
